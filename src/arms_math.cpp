/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   arms_math.cpp
 *
 *  \brief
 *    Implementation of simple math functionality
 */

#include <math.h>

#include "arms_math.h"

using namespace std;

Vec2::Vec2()
  : x(0), y(0) { }

Vec2::Vec2(float _x, float _y)
  : x(_x), y(_y)
{
}

Vec2::Vec2(const Vec2 &other)
{
  *this = other;
}

Vec2 &Vec2::operator=(const Vec2 &other)
{
  x = other.x;
  y = other.y;
  return *this;
}
Vec2 Vec2::operator*(const Vec2 &other) const
{
  Vec2 newVec = {0.0, 0.0};
  newVec.x = x * other.x;
  newVec.y = y * other.y;
  return newVec;
}
Vec2 Vec2::operator*(const float &scalar) const
{
  Vec2 newVec = {0.0, 0.0};
  newVec.x = x * scalar;
  newVec.y = y * scalar;
  return newVec;
}
Vec2 Vec2::operator/(const Vec2 &other) const
{
  Vec2 newVec = {0.0, 0.0};
  newVec.x = x / other.x;
  newVec.y = y / other.y;
  return newVec;
}
Vec2 Vec2::operator/(const float &scalar) const
{
  Vec2 newVec = {0.0, 0.0};
  newVec.x = x / scalar;
  newVec.y = y / scalar;
  return newVec;
}
Vec2 Vec2::operator+(const Vec2 &other) const
{
  Vec2 newVec = {0.0, 0.0};
  newVec.x = x + other.x;
  newVec.y = y + other.y;
  return newVec;
}
Vec2 Vec2::operator-(const Vec2 &other) const
{
  Vec2 newVec = {0.0, 0.0};
  newVec.x = x - other.x;
  newVec.y = y - other.y;
  return newVec;
}

bool Vec2::operator==(const Vec2 &other) const
{
  return x == other.x && y == other.y;
}
bool Vec2::operator!=(const Vec2 &other) const
{
  return x != other.x || y != other.y;
}

void Vec2::normalize()
{
  float scalar = 1 / magnitude(); 
  x *= scalar;
  y *= scalar;
}

float Vec2::magnitude()
{
  return sqrt(x * x + y * y);
}

float Vec2::dot(const Vec2 &other) const
{
  return x * other.x + y * other.y;
}

Vec3::Vec3(float _x, float _y, float _z)
  : x(_x), y(_y), z(_z)
{
}

Vec4::Vec4(float _x, float _y, float _z, float _w)
  : x(_x), y(_y), z(_z), w(_w)
{
}

float map_range_to(const float &value, const float &valueMin
    , const float &valueMax, const float &mapMin, const float &mapMax)
{
  return (value - valueMin) / (valueMax - valueMin) 
    * (mapMax - mapMin) + mapMin;
}
