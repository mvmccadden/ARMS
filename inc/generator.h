/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   generator.h
 *
 *  \brief
 *    Implementation file for generating and calculating data
 */

#pragma once

#include <cmath>

#include "parsedata.h"
#include "arms_math.h"

const Vec2 DEFAULT_ROOM_SIZE = {1000.f, 1000.f};
const float DEFAULT_RAY_DISTANCE = std::sqrt(DEFAULT_ROOM_SIZE.x 
    * DEFAULT_ROOM_SIZE.x + DEFAULT_ROOM_SIZE.y * DEFAULT_ROOM_SIZE.y);

struct CollisionInfo
{
  CollisionInfo()
    : collision(false), parent(nullptr), parentLine(0)
      , lineBegin({0.f, 0.f}), lineEnd({0.f, 0.f})
  {
  }
  CollisionInfo(bool _collision, Object *_parent, int _parentLine
      , const Vec2 &_lineBegin, const Vec2 &_lineEnd)
    : collision(_collision), parent(_parent), parentLine(_parentLine)
      , lineBegin(_lineBegin), lineEnd(_lineEnd)
  {
  }

  bool collision;
  Object *parent;
  int parentLine;
  Vec2 lineBegin;
  Vec2 lineEnd;
};

/*!
 *  Using the user defined room size will resize scene to correct aspect ratio
 *  with largest side being set to 500 and the smaller side being scaled in
 *  ratio to it so that it always fits within the application
 *
 *  \param dataMap
 *    A pointer to the dataMap with the parsed scene data
 *
 *  \returns
 *    A Vec4 containing the new scene room size scaled based on the user data 
 *    in position x and position y and the scalar in position z(x) and w(y)
 */
Vec4 get_room_size(DataMap *dataMap);

std::array<Vec2, 2> get_object_data(DataMap::DataMapIterator it);

std::vector<Object *> convert_DataMap_to_Object(DataMap *dataMap
    , const Vec2 &posOffset, const Vec2 &scalar);

std::vector<std::vector<AudioRay *>> generate_audio_rays_from_scene(
    std::vector<Object *> &objVec, const Vec2 &relativePos
    , const Vec2& relativeSize, const Vec2 &scalar);
