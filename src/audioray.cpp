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

AudioRay::AudioRay(Object *_parent, const int &_line, const float &amp
    , const Vec2 &_posA, const Vec2 &_posB)
    : parent(_parent), parentLine(_line), amplitude(amp)
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

float AudioRay::get_amp() const
{
  return amplitude;
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

void AudioRay::set_amp(const float &amp)
{
  amplitude = amp;
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
