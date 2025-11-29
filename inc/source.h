/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   source.h
 *
 *  \brief
 *    Interface file for source object
 */

#pragma once

#include "object.h"

/*!
 *  \class Source
 *  
 *  \biref
 *    An object used to source an audio wave to a listener
 */
class Source : public Object
{
  public:
    Source(const Vec2 &pos, const Vec2 &size);
    ~Source();
    
  private:
    inline static const sf::Color sourceColor = sf::Color::Blue;
};
