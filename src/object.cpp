/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   object.cpp
 *
 *  \brief
 *    Implementation file that handles defining what an object is
 */

#include "object.h"
#include <SFML/Graphics/RenderWindow.hpp>

using namespace std;

Object::Object(const Vec2 &_pos, const Vec2 &_size
    , const std::string &_typeName)
  : typeName(_typeName), position(_pos), size(_size)
    , drawBox({size.w, size.h})
{
  drawBox.setPosition({position.x, position.y});
}

Object::~Object() { }

void Object::draw(sf::RenderWindow &window) 
{
  window.draw(drawBox);
}

std::string Object::get_type_name() const
{
  return typeName;
}

const CArray<Vec2> &Object::get_absortion_coefficent() const
{
  return absortionCoefficents; 
}

void Object::set_position(const Vec2 &_pos)
{
  position = _pos;
  drawBox.setPosition({_pos.x, _pos.y});
}
void Object::set_size(const Vec2 &_size)
{
  size = _size;
  drawBox.setSize({size.x, size.y});
}

void Object::set_color(const sf::Color &color)
{
  drawBox.setFillColor(color);
}

const Vec2 &Object::get_position() const
{
  return position;
}
const Vec2 &Object::get_size() const
{
  return size;
}
