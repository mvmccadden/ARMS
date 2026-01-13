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
    AudioRay(Object *parent, const int &line, const CArray<Vec2> &amp
        , const Vec2 &_posA, const Vec2 &_posB);
    ~AudioRay();

    Object *get_parent() const;
    int get_parent_line() const;

    Vec2 get_posA() const;
    Vec2 get_posB() const;

    const CArray<Vec2> &get_amp() const;
    float get_distance() const;

    void set_parent_line(const int &line);

    void set_posA(const Vec2 &_posA);
    void set_posB(const Vec2 &_posB);
    void set_amp(const CArray<Vec2> &amp);
    void scale_amp(const float &scale);
    void add_to_amp(const CArray<Vec2> &amp);
    void scale_to_amp(const CArray<Vec2> &amp, const float &scale);
    void scale_and_add_to_amp(const CArray<Vec2> &amp, const float &scale);
    CArray<Vec2> add_amps(const CArray<Vec2> &amp);
    void inverse_amp();

    float get_amp_average();

    void set_color(const sf::Color &color);

    void draw(sf::RenderWindow &window) const;

  private:
    inline static const float lineWidth = 2.f;
  
    Object *parent;
    int parentLine;
    CArray<Vec2> coefficents;
    std::array<sf::Vertex, 2> line;
};
