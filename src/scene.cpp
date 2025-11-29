/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   scene.cpp
 *
 *  \brief
 *    Implementation of object and filter generation from scenes
 */

#include "scene.h"

#include "audioray.h"
#include "object.h"

#include "wave.h"
#include "filter.h"

#include "parsedata.h"
#include "generator.h"
#include <algorithm>

using namespace std;

Scene::Scene(const string &fileName, const Vec2 &topLeftPos, const Vec2 &size
    , const bool &ignoreInputDir)
  : relativePos(topLeftPos), relativeSize(size), currentSamplingRate(0u)
    , filter(nullptr)
{
  open_scene(fileName, ignoreInputDir);
}

Scene::~Scene() 
{
  clear();
}

void Scene::open_scene(const string &fileName, const bool &ignoreInputDir)
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

  RayGenerationInfo info;
  objects = convert_DataMap_to_Object(dataMap, relativePos, info);
  audioRayVec = generate_audio_rays_from_scene(info, objects
      , relativePos, relativeSize);
}

void Scene::apply_filter_to_wave(WaveFile &wave)
{
  unsigned incomingSamplingRate = wave.get_sampling_rate();
  if(currentSamplingRate != incomingSamplingRate)
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

void Scene::generate_scene_filter()
{
  filter = new Filter();

  for(vector<AudioRay *> audioRays : audioRayVec)
  {
    float distance = 0.f;
    // NOTE: each pixel is assumed to be a centimeter in this simulation atm
    for(AudioRay * audioRay : audioRays)
    { 
      distance += audioRay->get_distance();
    }
    float seconds = distance / 34300.f;
    // TODO: Each delay should be readjustable based on given filter
    unsigned delay = seconds * currentSamplingRate;
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
  delete filter;

  for(vector<AudioRay *> audioRays : audioRayVec) 
    for(AudioRay *audioRay : audioRays) 
      if (audioRay) delete audioRay;

  for(Object *object : objects) 
    if(object) delete object;
}
