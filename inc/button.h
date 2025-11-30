/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   button.h
 *
 *  \brief
 *    Interface of button handling
 */

#include <SFML/Graphics/Text.hpp>
#include <functional>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>

#include "object.h"
#include "arms_math.h"

class Button : public Object
{
  public:
    using BUTTON_ACTION = std::function<void()>;

    Button(const std::string &title, const Vec2 &_pos
        , const Vec2 &_size, BUTTON_ACTION action_func);
    ~Button();

    /*!
     *  Initalize the font for the button class
     *
     *  \param fontPath
     *    The file path to the font being used for the button
     */
    static void set_font(const std::string &fontPath);

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

    inline static sf::Font font;

    inline static const size_t textSize = 12;

    inline static const sf::Color defaultColor = sf::Color(200, 200, 200);
    inline static const sf::Color hoveredColor = sf::Color(100, 100, 100);
    inline static const sf::Color clickedColor = sf::Color(100, 100, 100);
};
