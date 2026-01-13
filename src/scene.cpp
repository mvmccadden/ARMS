/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   scene.cpp
 *
 *  \brief
 *    Implementation of object and filter generation from scenes
 */

#include "scene.h"

#include <SFML/System/Sleep.hpp>
#include <cmath>
#include <random>

#include "audioray.h"
#include "barrier.h"
#include "helper.h"
#include "object.h"

#include "wave.h"
#include "filter.h"

#include "parsedata.h"
#include "generator.h"

using namespace std;

Scene::Scene(const Vec2 &topLeftPos, const Vec2 &size, const Vec2 &scalar)
  : relativePos(topLeftPos), relativeSize(size), relativeScalar(scalar) { }

Scene::Scene(const string &fileName, const Vec2 &topLeftPos, const Vec2 &size
    , const Vec2 &scalar, const bool &ignoreInputDir)
  : relativePos(topLeftPos), relativeSize(size), currentSamplingRate(0u)
    , relativeScalar(scalar)
{
  open_scene(fileName, ignoreInputDir);
}

Scene::~Scene() 
{
  clear();
}

bool Scene::is_open() const
{
  return open;
}

Vec2 Scene::open_scene(const string &fileName, const bool &ignoreInputDir)
{
  clear();

  if(ignoreInputDir)
  {
    size_t lastSlash = fileName.find_last_of("\\/");
    if(lastSlash != string::npos) 
    {
      name = fileName.substr(lastSlash);
    }
    else
    {
      name = "output";
    }

    size_t dotPos = name.rfind('.');
    if(dotPos != string::npos)
    {
      name = name.substr(0, dotPos);
    }
  }
  else
  {
    name = fileName;
  }

  DataMap *dataMap = read_scene_file(fileName, ignoreInputDir);

  if(dataMap == nullptr)
  {
    static_cast<void>(Logger(Logger::L_ERR, "Invalid map was created"));
  }

  open = true;
  
  Vec4 roomData = get_room_size(dataMap);
  relativeSize = Vec2{roomData.x, roomData.y};
  relativeScalar = Vec2{roomData.z, roomData.w};

  objects = convert_DataMap_to_Object(dataMap, relativePos, relativeScalar);
  audioRayVec = generate_audio_rays_from_scene(objects, relativePos
      , relativeSize, relativeScalar);

  return relativeSize;
}

void Scene::apply_filter_to_wave(WaveFile &wave)
{
  unsigned incomingSamplingRate = wave.get_sampling_rate();
  if((filters.size() == 0) || currentSamplingRate != incomingSamplingRate)
  {
    if(filters.size() > 0)
    {
      filters.clear();
    }
    currentSamplingRate = incomingSamplingRate;
    generate_scene_filter();
  }

  CArray<float> output;
  for(size_t i = 0; i < filters.size(); ++i)
  {
    CArray<float> input(wave.get_samples());
    filters[i].apply_filter(input);
    output += input;
  }
  wave.get_samples() = output;
}

/*!
 *  Equation:
 *    A(t) = A_0 * e^((-6.908 * t)/T60)
 *
 *    -6.908 is the ln(1000) which represents the drop to -60dB which is
 *    1/1000th the amplitude of the original signal.
 *
 *    We will use a mix of comb and lowpass filters to creat artifical
 *    reverberation. To avoid robotic sounding delay we will use Schroeder
 *    Reverb with 4 parallel delay lines with coprime delay lengths
 *
 *    T60 is taken from the Sabine formula for RT_60 = (0.161 * V) / A
 *    V = room volume in m^3
 *    A = total absorbtion area in Sabins (the sum of all surface areas
 *      multiplied by their respective absortion coefficients)
 *
 *    We then move this into 2D space by replace V with A2 and A with P
 *    A2 = room area in m^2
 *    P = perimeter of all surfaces multiplied by their respective abosorbtion
 *      coefficients
 */
void Scene::apply_t60_to_wave(WaveFile &wave)
{
  if(currentSamplingRate != wave.get_sampling_rate())
  {
    currentSamplingRate = wave.get_sampling_rate();
  }

  // Using an audio ray to track absorbtion as it has all the built in
  // functionality needed
  AudioRay absorbtionRay(nullptr, 0, Barrier::get_coefficent(Barrier::C_WALL)
      , {0.f, 0.f}, {0.f, 0.f});
  absorbtionRay.scale_amp(2.f * relativeSize.x 
      + 2.f * relativeSize.y);
  for(size_t i = 0; i < absorbtionRay.get_amp().size(); ++i)
  {
    Logger(Logger::L_MSG, "T60 AR" + to_string(i) + ": " 
        + to_string(absorbtionRay.get_amp().at(i).y));
  }

  float area = relativeSize.x * relativeSize.y;

  for(Object *object : objects)
  {
    Vec2 size = object->get_size();
    absorbtionRay.scale_and_add_to_amp(object->get_absortion_coefficent()
        , 2.f * size.x + 2.f *size.y);
  }

  Logger(Logger::L_MSG, "T60 AMP AVERAGE: " 
      + to_string(absorbtionRay.get_amp_average()));

  CArray<Vec2> bands(absorbtionRay.get_amp());
  for(size_t i = 0; i < bands.size(); ++i)
  {
    // e^(-6.908)/T60
    bands[i].y = exp((-6.908f) 
        / ((0.161f * area) / (bands[i].y)));
    Logger(Logger::L_MSG, "T60 BAND " + to_string(i) + ": " 
        + to_string(bands[i].y));
  }


  // TODO: Handle extra space needed for samples still reverberating 
  // i.e. if a given wav is too short and we need more samples to fully
  // show reverberance
  
  // NOTE: Each pixel is a centimeter atm

  // delay is in ms and uses the larger wall distance to calculate the ms delay
  uint16_t delayTime = (relativeSize.x > relativeSize.y)
    ? relativeSize.x / 34300.f * wave.get_sampling_rate()
    : relativeSize.y / 34300.f * wave.get_sampling_rate();

  CArray<float> output;
  // Random number generator for getting other delays
  random_device randomDevice;
  default_random_engine randomEngine(randomDevice());
  uniform_int_distribution<int> randomAdd(-20, 20);
  // Generate delay lines and apply them to the output
  const size_t delayCount = 10;
  for(size_t i = 0; i < delayCount; ++i)
  {
    static_cast<void>(Logger(Logger::L_MSG, "T60 DelayTime " + to_string(i)
          + ": " + to_string(delayTime)));

    CArray<float> input(wave.get_samples());
    
    size_t bandSize = bands.size();
    Equalizer t60EQ(bandSize, currentSamplingRate, delayTime);
    for(size_t j = 0; j < bandSize; ++j)
    {
      t60EQ.add_coefficent(bands[j].x, bands[j].y / delayCount, j);
    }

    t60EQ.apply_filter(input);
    output += input;

    // Generate a new randomly generated delay to offset delays and make sound
    // more natural
    delayTime += randomAdd(randomEngine);
  }

  // Overwrite samples from output into wave object
  wave.get_samples() = output;
}

void Scene::draw(sf::RenderWindow &window)
{
  for(vector<AudioRay *> audioRays : audioRayVec)
  {
    for(AudioRay *audioRay : audioRays)
    {
      audioRay->draw(window);
    }
  }

  for(Object *object : objects)
  {
    object->draw(window);
  }
}

// TODO: Add comb delay and allpass 
void Scene::add_bandpass_reverb_filter(const uint16_t &delay
    , const float &frequency, const size_t &bandCount
    , const float &coefficent, const float &outputScale
    , const CArray<float> &input, CArray<float> &output)
{
  CArray<float> delayLine(input);
  float bandwidth = currentSamplingRate / 2.f / bandCount;
  BandPass band(frequency, 0.1f);
  band.set_sampling_rate(currentSamplingRate);
  band.update_values();
  band.set_gain(1.f);
  band.apply_filter(delayLine);

  // Ensure the output and delayLine size account for the added delay
  delayLine.resize(delayLine.size() + delay);
  if(output.size() < delayLine.size())
  {
    output.resize(delayLine.size());
  }

  // Go through samples and adjust based on T60 calculation
  float echoOutput = 0.f;
  // Ignore the first [delayTime] number of samples as they will be 0
  for(int i = delay; i < delayLine.size(); ++i)
  {
    float echo = delayLine[i-delay];
    float lowpassEcho = (echo + echoOutput) * 0.5f;
    echoOutput = lowpassEcho;
    float outputValue = lowpassEcho * coefficent;

    // As we are adding another sample to an existing sample we must scale
    // it down so that it accuratley represents the new mixxed signal
    delayLine[i] = (delayLine[i] + outputValue) * 0.5f;
    output[i] += outputValue * outputScale;
    if(output[i] > 1.f || output[i] < -1.f || output[i] == NAN)
    {
      Logger(Logger::L_WRN, "ERROR INVALID OUTPUT " + to_string(output[i]));
    }
  }
}

void Scene::generate_scene_filter()
{
  filters.clear();

  size_t i = 0;
  for(vector<AudioRay *> &audioRays : audioRayVec)
  {
    float distance = 0.f;
    // NOTE: each pixel is assumed to be a centimeter in this simulation atm
    for(AudioRay *audioRay : audioRays)
    { 
      distance += audioRay->get_distance();
    }
    unsigned delay = distance / 34300.f * currentSamplingRate;

    const CArray<Vec2> &array = audioRays.back()->get_amp();
    filters[i] = Equalizer(array.size(), currentSamplingRate, delay);

    for(size_t j = 0; j < array.size(); ++j)
    {
      // Divide the coefficent by the number of rays to ensure it doesn't get
      // overloaded
      filters[i].add_coefficent(array.at(j).x, array.at(j).y / audioRays.size()
          , static_cast<unsigned>(j));
    }

    ++i;;
  }
}

string Scene::get_name() const
{
  return name;
}

void Scene::clear()
{
  filters.clear();
  currentSamplingRate = 0;

  for(vector<AudioRay *> audioRays : audioRayVec) 
    for(AudioRay *audioRay : audioRays) 
      if (audioRay) delete audioRay;

  audioRayVec.clear();

  for(Object *object : objects) 
    if(object) delete object;

  objects.clear();
}
