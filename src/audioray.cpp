/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   audioray.cpp
 *
 *  \brief
 *    Implementation of audio ray object 
 */

#include <SFML/Graphics/PrimitiveType.hpp>

#include <cmath>

#include "audioray.h"
#include "helper.h"

AudioRay::AudioRay(Object *_parent, const int &_line, const CArray<Vec2> &amp
    , const Vec2 &_posA, const Vec2 &_posB)
    : parent(_parent), parentLine(_line), coefficents(amp)
{
  line[0].position = {_posA.x, _posA.y};
  line[0].color = sf::Color(0, 255, 0);
  line[1].position = {_posB.x, _posB.y};
  line[1].color = sf::Color(0, 255, 0);
}

AudioRay::~AudioRay() { }

Object *AudioRay::get_parent() const
{
  return parent;
}

int AudioRay::get_parent_line() const
{
  return parentLine;
}

Vec2 AudioRay::get_posA() const
{
  return {line[0].position.x, line[0].position.y};
}

Vec2 AudioRay::get_posB() const
{
  return {line[1].position.x, line[1].position.y};
}

const CArray<Vec2> &AudioRay::get_amp() const
{
  return coefficents;
}

float AudioRay::get_distance() const
{
  float a = line[1].position.x - line[0].position.x;
  float b = line[1].position.y - line[0].position.y;

  return sqrt(a * a + b * b);
}

void AudioRay::set_parent_line(const int &_line)
{
  parentLine = _line;
}
  
void AudioRay::set_posA(const Vec2 &_posA)
{
  line[0].position = {_posA.x, _posA.y};
}

void AudioRay::set_posB(const Vec2 &_posB)
{
  line[1].position = {_posB.x, _posB.y};
}

void AudioRay::set_amp(const CArray<Vec2> &amp)
{
  coefficents = amp;
}

void AudioRay::scale_amp(const float &scale)
{
  for(size_t i = 0; i < coefficents.size(); ++i)
  {
    coefficents[i].y *= scale;
  }
}

void AudioRay::scale_and_add_to_amp(const CArray<Vec2> &amp, const float &scale)
{
  for(size_t i = 0; i < amp.size(); ++i)
  {
    // Check to see if there already exists this frequency and then just
    // multiply the value against the existings value
    bool newFreq = true;
    float freq = amp.at(i).x;
    float coefficent = amp.at(i).y;

    for(size_t j = 0; j < coefficents.size(); ++j)
    {
      if(coefficents[j].x == amp.at(i).x)
      {
        coefficents[j].y += scale * (1.f - coefficent);
        newFreq = false;
        break;
      }
    }

    // If the frequency doesn't already exists then add it
    if(newFreq)
    {
      coefficents.resize(coefficents.size() + 1);
      coefficents[coefficents.size() - 1] = {freq, (1.f - coefficent) * scale };
    }
  }
}

void AudioRay::scale_to_amp(const CArray<Vec2> &amp, const float &scale)
{
  for(size_t i = 0; i < amp.size(); ++i)
  {
    // Check to see if there already exists this frequency and then just
    // multiply the value against the existings value
    bool newFreq = true;
    float freq = amp.at(i).x;
    float coefficent = amp.at(i).y;

    for(size_t j = 0; j < coefficents.size(); ++j)
    {
      if(coefficents[j].x == amp.at(i).x)
      {
        coefficents[j].y *= scale * (1.f - coefficent);
        newFreq = false;
        break;
      }
    }

    // If the frequency doesn't already exists then add it
    if(newFreq)
    {
      coefficents.resize(coefficents.size() + 1);
      coefficents[coefficents.size() - 1] = {freq, (1.f - coefficent) * scale };
    }
  }
}

void AudioRay::inverse_amp()
{
  for(size_t i = 0; i < coefficents.size(); ++i)
  {
    coefficents[i].y = 1.f - coefficents[i].y;
  }
}

void AudioRay::add_to_amp(const CArray<Vec2> &amp)
{
  for(size_t i = 0; i < amp.size(); ++i)
  {
    // Check to see if there already exists this frequency and then just
    // multiply the value against the existings value
    bool newFreq = true;
    float freq = amp.at(i).x;
    float coefficent = amp.at(i).y;

    for(size_t j = 0; j < coefficents.size(); ++j)
    {
      if(coefficents[j].x == freq)
      {
        coefficents[j].y *= 1.f - coefficent;
        newFreq = false;
        break;
      }
    }

    // If the frequency doesn't already exists then add it
    if(newFreq)
    {
      coefficents.resize(coefficents.size() + 1);
      coefficents[coefficents.size() - 1] = {freq, 1.f - coefficent};
    }
  }
}

CArray<Vec2> AudioRay::add_amps(const CArray<Vec2> &amp)
{
  CArray<Vec2> returnVec(coefficents);

  for(size_t i = 0; i < amp.size(); ++i)
  {
    // Check to see if there already exists this frequency and then just
    // multiply the value against the existings value
    bool newFreq = true;
    float freq = amp.at(i).x;
    float coefficent = amp.at(i).y;

    for(size_t j = 0; j < returnVec.size(); ++j)
    {
      if(returnVec[j].x == amp.at(i).x)
      {
        returnVec[j] = {
          freq, coefficents[j].y * (1.f - coefficent)
        };
        newFreq = false;
        break;
      }
    }

    // If the frequency doesn't already exists then add it
    if(newFreq)
    {
      returnVec.resize(returnVec.size() + 1);
      returnVec[returnVec.size() - 1] = {freq, 1.f - coefficent};
    }
  }

  return returnVec;
}

float AudioRay::get_amp_average()
{
  float average = 0.f;
  for(size_t i = 0; i < coefficents.size(); ++i)
  {
    average += coefficents[i].y;
  }

  /*
  if(coefficents.size() == 0)
  {
    Logger(Logger::L_WRN
        , "Getting average amp of a AudioRay with 0 coefficents");
  }
  */

  return (coefficents.size() == 0) ? 1 : average / coefficents.size();
}

void AudioRay::set_color(const sf::Color &color)
{
  line[0].color = color;
  line[1].color = color;
}

void AudioRay::draw(sf::RenderWindow &window) const
{
  window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
}
