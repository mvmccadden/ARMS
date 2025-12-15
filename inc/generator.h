/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   generator.h
 *
 *  \brief
 *    Implementation file for generating and calculating data
 */

#pragma once

#include "parsedata.h"
#include "arms_math.h"

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


std::array<Vec2, 2> get_object_data(DataMap::DataMapIterator it);

std::vector<Object *> convert_DataMap_to_Object(DataMap *dataMap
    , const Vec2 &posOffset, RayGenerationInfo &info);

std::vector<std::vector<AudioRay *>> generate_audio_rays_from_scene(
    const RayGenerationInfo &info, std::vector<Object *> &objVec
    , const Vec2 &relativePos, const Vec2& relativeSize);
