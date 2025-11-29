/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   listener.h
 *
 *  \brief
 *    Interface file for listener object
 */

#pragma once

#include "object.h"

/*!
 *  \class Listener
 *
 *  \brief
 *    An object used to listen for audio sources
 */
class Listener : public Object
{
  public:
    Listener(const Vec2 &pos, const Vec2 &size, const std::string &pattern);
    ~Listener();

  private:
    inline static const sf::Color listenerColor = sf::Color::Red;

    struct PatternInfo
    {
      std::string pattern;
      float direction;
    } patternInfo;
};

