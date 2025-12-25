/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   source.cpp
 *
 *  \brief
 *    Implementation file for source object
 */

#include "source.h"

Source::Source(const Vec2 &pos, const Vec2 &size, const float &_direction
    , const float &_cone, const int &_checks, const int &_rays)
  : Object(pos, size, "Source"), direction(_direction), cone(_cone)
    , checks(_checks), rays(_rays)
{
  set_color(sourceColor);
  absortionCoefficent = 0.f;
}

Source::~Source() { }

const float &Source::get_direction()
{
  return direction;
}

const float &Source::get_cone()
{
  return cone;
}

const int &Source::get_checks()
{
  return checks;
}


const int &Source::get_rays()
{
  return rays;
}
