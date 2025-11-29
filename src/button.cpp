/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   button.h
 *
 *  \brief
 *    Implementation of button handling
 */

#include "button.h"
#include "helper.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>

using namespace std;

Button::Button(const std::string &title, const Vec2 &_pos
    , const Vec2 &_size, BUTTON_ACTION action_func)
  : Object(_pos, _size, "Button"), buttonState(BS_DEFAULT)
    , action(action_func), buttonText(font) 
{ 
  buttonText.setString(title);
  buttonText.setFillColor(sf::Color::Black);
  resize_button();

  drawBox.setFillColor(defaultColor);
}

Button::~Button() { }

void Button::set_action(BUTTON_ACTION *action_func)
{
  action = *action_func;
}

void Button::set_title(const std::string &title)
{
  buttonText.setString(title);
}

void Button::draw(sf::RenderWindow &window) 
{
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);

  if(mousePos.x > position.x && mousePos.x < position.x + size.x
      && mousePos.y > position.y && mousePos.y < position.y + size.y)
  {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
      if(buttonState != BS_CLICKED)
      {
        buttonState = BS_CLICKED;
        drawBox.setFillColor(clickedColor);
      }
    }
    else if(buttonState == BS_CLICKED)
    {
      action();
      buttonState = BS_HOVERED;
      drawBox.setFillColor(hoveredColor);
    }
    else if(buttonState != BS_HOVERED)
    {
      buttonState = BS_HOVERED;
      drawBox.setFillColor(hoveredColor);
    }
  }
  else if(buttonState != BS_DEFAULT)
  {
    buttonState = BS_DEFAULT;
    drawBox.setFillColor(defaultColor);
  }

  window.draw(drawBox);
  window.draw(buttonText);
}

void Button::set_font(const string &fontPath)
{
  if(!font.openFromFile(FONT_DIR + fontPath))
  {
    Logger(Logger::L_ERR, "Invalid font path given");
  }
}

void Button::resize_button()
{
  sf::FloatRect textBounds = buttonText.getLocalBounds();

  size_t numOfChars = buttonText.getString().getSize();
  unsigned textSize = size.x / textBounds.size.x 
    * buttonText.getCharacterSize();
  if(textSize > size.y - 8.f)
  {
    textSize = size.y - 8.f;
  }

  buttonText.setCharacterSize(textSize);
  textBounds = buttonText.getLocalBounds();

  Vec2 sizeDiff = {((size.x - textBounds.size.x) / 2.f)
    , (size.y - textSize) / 2.f};

  Logger(Logger::L_MSG, "TEXT: " + buttonText.getString() + ", with size: "
    + to_string(numOfChars));

  buttonText.setPosition({position.x + sizeDiff.x, position.y + sizeDiff.y});
}
