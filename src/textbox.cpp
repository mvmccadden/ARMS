/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   textbox.cpp
 *
 *  \brief
 *    Implementation file for textbox objects
 */

#include "textbox.h"
#include "text.h"

using namespace std;

TextBox::TextBox(const std::string &title, const Vec2 &_pos, const Vec2 &_size)
  : Object(_pos, _size, "TextBox"), boxText(get_font())
{
  boxText.setString(title);
  boxText.setFillColor(sf::Color::Black);
  resize_box();

  drawBox.setFillColor(textBoxColor);
}

TextBox::~TextBox() { }

void TextBox::set_title(const string &title)
{
  boxText.setString(title);
  resize_box();
}

void TextBox::draw(sf::RenderWindow &window)
{
  window.draw(drawBox);
  window.draw(boxText);
}

void TextBox::resize_box()
{
  boxText.setCharacterSize(textSize);

  sf::FloatRect textBounds = boxText.getLocalBounds();
  Vec2 sizeDiff = {((size.x - textBounds.size.x) / 2.f)
    , (size.y - textSize) / 2.f};

  static_cast<void>(Logger(Logger::L_MSG, "TEXT: " + boxText.getString() 
        + ", with size: " + to_string(textSize)));

  bool addElipse;
  float elipseSize = get_elipse_size();

  // 4.f is how much space we want on either side...
  // If we are doing this we also want to add a '...' to show
  // that the name is supposed to have more so we will account
  // for that in the sizing in of the size diff
  while(sizeDiff.x < 4.f)
  {
    boxText.setString(boxText.getString().substring(0
          , boxText.getString().getSize() - 1));

    textBounds = boxText.getLocalBounds();
    sizeDiff = {((size.x - textBounds.size.x - elipseSize) / 2.f)
      , (size.y - textSize) / 2.f};

    addElipse = true;
  }

  // Add elipse in post to that this isn't the full name
  if(addElipse)
  {
    boxText.setString(boxText.getString() + "...");
  }

  boxText.setPosition({position.x + 4.f, position.y + sizeDiff.y});
}
