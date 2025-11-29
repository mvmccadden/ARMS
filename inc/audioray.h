/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   audioray.h
 *
 *  \brief
 *    Interface of audio ray object 
 */

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "arms_math.h"
#include "object.h"

#define NULL_LINES 999
#define DEFAULT_AMP 1.f

/*!
 *  \class AudioRay
 *
 *  \brief
 *    A visual representation of an audio wave in ray form
 */
class AudioRay 
{
  public:
    // NOTE: Repurposing size as posB in line and normal pos and posA
    AudioRay(Object *parent, const int &line, const float &amp
        , const Vec2 &_posA, const Vec2 &_posB);
    ~AudioRay();

    Object *get_parent() const;
    int get_parent_line() const;

    Vec2 get_posA() const;
    Vec2 get_posB() const;

    float get_amp() const;
    float get_distance() const;

    void set_parent_line(const int &line);

    void set_posA(const Vec2 &_posA);
    void set_posB(const Vec2 &_posB);

    void set_color(const sf::Color &color);

    void draw(sf::RenderWindow &window) const;

  private:
    inline static const float lineWidth = 2.f;
  
    Object *parent;
    int parentLine;
    float amplitude;
    std::array<sf::Vertex, 2> line;
};
