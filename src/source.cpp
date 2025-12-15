/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   source.cpp
 *
 *  \brief
 *    Implementation file for source object
 */

#include "source.h"

Source::Source(const Vec2 &pos, const Vec2 &size)
  : Object(pos, size, "Source")
{
  set_color(sourceColor);
  absortionCoefficent = 0.f;
}

Source::~Source() { }
