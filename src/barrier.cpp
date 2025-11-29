/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   audioray.cpp
 *
 *  \brief
 *    Implementation of audio ray object 
 */

#include "barrier.h"

using namespace std;

Barrier::Barrier(const Vec2 &pos, const Vec2 &size, const string &_type)
  : Object(pos, size, "Barrier"), type(_type)
{
  if(type == "wood")
  {
    set_color(sf::Color::Yellow);
    resistanceCoefficent = .8f;
  }
  else if(type == "rubber")
  {
    set_color(sf::Color::Magenta);
    resistanceCoefficent = .4f;
  }
  else if(type == "wall")
  {
    set_color(sf::Color::Transparent);
    resistanceCoefficent = .5f;
  }
  else
  {
    set_color(sf::Color::Black);
    resistanceCoefficent = 1.f;
  }
}

Barrier::~Barrier() { }

const string &Barrier::get_type_data()
{
  return type;
}
