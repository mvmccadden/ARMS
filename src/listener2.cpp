/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   listener.h
 *
 *  \brief
 *    Implementation file for listener object
 */

#include "listener2.h"

Listener::Listener(const Vec2 &pos, const Vec2  &size
    , const std::string &_pattern)
  : Object(pos, size, "Listener")
{
  set_color(listenerColor);
  resistanceCoefficent = 1.f;

  patternInfo.pattern = _pattern;

  if(patternInfo.pattern == "Omni")
  {
    patternInfo.direction = 0.f;
  }
}

Listener::~Listener() { }
