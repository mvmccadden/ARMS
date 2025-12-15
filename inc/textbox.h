/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   textbox.h
 *
 *  \brief
 *    Interface file for textbox objects
 */

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

#include "text.h"
#include "object.h"
#include "colors.h"
#include "arms_math.h"

class TextBox : public Object
{
  public:
    TextBox(const std::string &title, const Vec2 &_pos, const Vec2 &_size);
    ~TextBox();

    void set_title(const std::string &title);

    void draw(sf::RenderWindow &window);
  private:
    void resize_box();

    sf::Text boxText;

    inline static sf::Font font;

    inline static constexpr sf::Color textBoxColor = color4;
};
