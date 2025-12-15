/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   button.h
 *
 *  \brief
 *    Interface of button handling
 */

#pragma once

#include <functional>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

#include "text.h"
#include "object.h"
#include "colors.h"
#include "arms_math.h"

class Button : public Object
{
  public:
    using BUTTON_ACTION = std::function<void()>;

    Button(const std::string &title, const Vec2 &_pos
        , const Vec2 &_size, BUTTON_ACTION action_func);
    ~Button();

    void set_action(BUTTON_ACTION *action_func);
    void set_title(const std::string &title);

    /*!
     *  Draws a button object onto the screen and performs updates based
     *  on mouse state and button position
     *
     *  \param window
     *    The window being rendered to
     *  \param mousePos
     *    The position of the mouse relative to the window
     */
    void draw(sf::RenderWindow &window) override;
  private:
    void resize_button();

    enum BUTTON_STATE
    {
      BS_DEFAULT = 0
      , BS_HOVERED
      , BS_CLICKED
    } buttonState;

    BUTTON_ACTION action;
    sf::Text buttonText;

    inline static const sf::Color defaultColor = color1;
    inline static const sf::Color hoveredColor = color2;
    inline static const sf::Color clickedColor = color3;
};
