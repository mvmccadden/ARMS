/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   scene.cpp
 *
 *  \brief
 *    Implementation of object and filter generation from scenes
 */

#include "scene.h"

#include <cmath>
#include <random>

#include "audioray.h"
#include "barrier.h"
#include "object.h"

#include "wave.h"
#include "filter.h"

#include "parsedata.h"
#include "generator.h"

using namespace std;

Scene::Scene(const Vec2 &topLeftPos, const Vec2 &size, const Vec2 &scalar)
  : relativePos(topLeftPos), relativeSize(size), relativeScalar(scalar)
    , filter(nullptr) { }

Scene::Scene(const string &fileName, const Vec2 &topLeftPos, const Vec2 &size
    , const Vec2 &scalar, const bool &ignoreInputDir)
  : relativePos(topLeftPos), relativeSize(size), currentSamplingRate(0u)
    , relativeScalar(scalar) , filter(nullptr)
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
  if(!filter || currentSamplingRate != incomingSamplingRate)
  {
    if(filter)
    {
      delete filter;
    }
    currentSamplingRate = incomingSamplingRate;
    generate_scene_filter();
  }

  filter->apply_filter(wave.get_samples());
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
  float area = relativeSize.x * relativeSize.y;
  float perimeters = (2.f * relativeSize.x + 2.f * relativeSize.y) 
    * Barrier::get_coefficent(Barrier::C_WALL);

  for(Object *object : objects)
  {
    Vec2 size = object->get_size();
    perimeters += (2.f * size.x + 2.f * size.y) 
      * object->get_absortion_coefficent();
  }

  const float T60 = (0.161f * area) / perimeters;
  // e^(-6.908)/T60
  const float coefficent = exp((-6.908f) / T60);
  static_cast<void>(Logger(Logger::L_MSG, "T60 Coefficent: "
        + to_string(coefficent)));

  // TODO: Handle extra space needed for samples still reverberating 
  // i.e. if a given wav is too short and we need more samples to fully
  // show reverberance
  
  // NOTE: Each pixel is a centimeter atm

  // delay is in ms and uses the larger wall distance to calculate the ms delay
  uint16_t delayTime = (relativeSize.x > relativeSize.y)
    ? relativeSize.x / 34300.f * wave.get_sampling_rate()
    : relativeSize.y / 34300.f * wave.get_sampling_rate();

  CArray<float> output;
  output.resize(wave.get_samples().size());
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

    add_reverb_filter(delayTime, 0.95f 
        , 1.f / static_cast<float>(delayCount), wave.get_samples(), output);

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
void Scene::add_reverb_filter(const uint16_t &delay, const float &coefficent
    , const float &outputScale, const CArray<float> &input
    , CArray<float> &output)
{
  CArray<float> delayLine(input);
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
    delayLine[i] += outputValue * 0.5f;
    output[i] += outputValue * outputScale;
  }
}

void Scene::generate_scene_filter()
{
  filter = new Filter();

  for(vector<AudioRay *> audioRays : audioRayVec)
  {
    float distance = 0.f;
    // NOTE: each pixel is assumed to be a centimeter in this simulation atm
    for(AudioRay *audioRay : audioRays)
    { 
      distance += audioRay->get_distance();
    }
    unsigned delay = distance / 34300.f * currentSamplingRate;
    // TODO: Each delay should be readjustable based on given filter
    float coefficent = audioRays.back()->get_amp();

    static_cast<void>(Logger(Logger::L_MSG
          , "Creating a filter coefficent with value: " + to_string(coefficent) 
          + ", and a delay of: " + to_string(delay)));
    filter->add_coefficent({coefficent, delay});
  }
}

string Scene::get_name() const
{
  return name;
}

void Scene::clear()
{
  if(filter) 
  {
    delete filter;
    currentSamplingRate = 0;
    filter = nullptr;
  }

  for(vector<AudioRay *> audioRays : audioRayVec) 
    for(AudioRay *audioRay : audioRays) 
      if (audioRay) delete audioRay;

  audioRayVec.clear();

  for(Object *object : objects) 
    if(object) delete object;

  objects.clear();
}
