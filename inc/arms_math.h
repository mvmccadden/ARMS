/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   arms_math.h
 *
 *  \brief
 *    Interface of simple math functionality
 */

#pragma once

struct Vec2
{
  Vec2(float _x, float _y);

  Vec2 &operator=(const Vec2 &other);
  Vec2 operator*(const Vec2 &other) const;
  Vec2 operator*(const float &scalar) const;
  Vec2 operator/(const Vec2 &other) const;
  Vec2 operator/(const float &scalar) const;
  Vec2 operator+(const Vec2 &other) const;
  Vec2 operator-(const Vec2 &other) const;

  bool operator==(const Vec2 &other) const;
  bool operator!=(const Vec2 &other) const;

  void normalize();
  float magnitude();
  float dot(const Vec2 &other) const;

  union
  {
    float x;
    float w;
  };
  union
  {
    float y;
    float h;
  };
};

struct Vec3
{
  Vec3(float _x, float _y, float _z);

  union
  {
    float x;
    float w;
    float r;
  };
  union
  {
    float y;
    float h;
    float g;
  };
  union
  {
    float z;
    float l;
    float b;
  };
};

float map_range_to(const float &value, const float &valueMin
    , const float &valueMax, const float &mapMin, const float &mapMax);
