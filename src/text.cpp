/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   text.cpp
 *
 *  \brief
 *    Implement file for textbox objects
 */

#include "text.h"
#include "helper.h"

sf::Font defaultFont;
float elipseSize;

void init_font()
{
  if(!defaultFont.openFromFile(DEFAULT_FONT))
  {
    Logger(Logger::L_ERR, "Invalid font path given");
  }

  sf::Text elipse(defaultFont);
  elipse.setString("...");
  sf::Vector2 bounds = elipse.getLocalBounds().size;
  elipseSize = bounds.x;
}

sf::Font &get_font()
{
  return defaultFont;
}

const float &get_elipse_size()
{
  return elipseSize;
}
