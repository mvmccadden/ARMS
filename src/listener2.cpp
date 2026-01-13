/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   listener.h
 *
 *  \brief
 *    Implementation file for listener object
 *
 *    Polar pattern equation for calculating gain based on ray direction.
 *
 *    g(d) = (1 - s) + s * dot(p, d)
 *
 *    d = The direction of the ray as a Vec2
 *    p = The direciton of the microphone as a Vec2
 *    s = The relationship of the omni portion of the patern vs the figure-8
 *        portion (0 = Omni, 0.5 = 50% Omni + 50% Figure-8, 1 = Figure-8)
 */

#include "listener2.h"

#include <math.h>

using namespace std;

// Doing inverse of direction as y-axis is flipped
Listener::Listener(const Vec2 &pos, const Vec2  &size, const float &direction
    , const std::string &pattern)
  : Object(pos, size, "Listener")
    , directionVec(cos(-direction), sin(-direction))
{
  directionVec.normalize();

  set_color(listenerColor);
  absortionCoefficents = CArray<Vec2>{{500.f, 0.f}};

  if(pattern == "omni")
  {
    polarPattern = P_OMNI;
  }
  else if(pattern == "sub")
  {
    polarPattern = P_SUBCARDIOID;
  }
  else if(pattern == "cardioid")
  {
    polarPattern = P_CARDIOID;
  }
  else if(pattern == "super")
  {
    polarPattern = P_SUPERCARDIOID;
  }
  else if(pattern == "hyper")
  {
    polarPattern = P_HYPERCARDIOID;
  }
  else if(pattern == "bi")
  {
    polarPattern = P_BIDIRECTIONAL;
  }
  else 
  {
    polarPattern = P_OMNI;
  }

  static_cast<void>(Logger(Logger::L_MSG, "Listener Pattern: " + pattern));
  static_cast<void>(Logger(Logger::L_MSG, "Listener S value: " 
      + to_string(PolarCoefficents[polarPattern])));
}

Listener::~Listener() { }

float Listener::get_directional_gain(Vec2 ray)
{
  float s = PolarCoefficents[polarPattern];
  ray.normalize();
  return abs((1 - s) + s * directionVec.dot(ray));
}
