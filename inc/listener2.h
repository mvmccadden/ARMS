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
    enum POLAR_PATTERNS
    {
      P_OMNI = 0
      , P_SUBCARDIOID
      , P_CARDIOID
      , P_SUPERCARDIOID
      , P_HYPERCARDIOID
      , P_BIDIRECTIONAL
      , P_COUNT
    };

    inline static const float PolarCoefficents[P_COUNT] =
    {
      0.f, 0.25f, 0.37f, 0.5f, 0.7f, 1.f
    };

    Listener(const Vec2 &pos, const Vec2 &size, const float &direction
        , const std::string &pattern);
    ~Listener();

    /*!
     *  Gets the direcitonal gain of a given ray based on the polar pattern
     *  of the listener
     *
     *  \param ray
     *    The direction vector of the ray being inputed
     */
    float get_directional_gain(Vec2 ray);

  private:
    inline static const sf::Color listenerColor = sf::Color::Red;

    Vec2 directionVec;
    POLAR_PATTERNS polarPattern = P_COUNT;
};

