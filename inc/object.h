/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   object.h
 *
 *  \brief
 *    Interface file that handles defining what an object is
 */

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "arms_math.h"
#include "helper.h"

/*!
 *  \class Object
 *
 *  \brief
 *    A generic class used to store an objects information
 *
 *  \operations
 *    set_position
 *    set_size
 *    get_pos
 *    get_size
 */
class Object
{
  public:
    Object(const Vec2 &_pos, const Vec2 &_size
        , const std::string &_typeName = "Object");
    virtual ~Object();

    virtual void draw(sf::RenderWindow &window);

    std::string get_type_name() const;

    const CArray<Vec2> &get_absortion_coefficent() const;

    void set_position(const Vec2 &pos);
    void set_size(const Vec2 &size);
    void set_color(const sf::Color &color);

    const Vec2 &get_position() const;
    const Vec2 &get_size() const;

  protected:
    CArray<Vec2> absortionCoefficents;
    Vec2 position;
    Vec2 size;

    sf::RectangleShape drawBox;

    std::string typeName;
};
