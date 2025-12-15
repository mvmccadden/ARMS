/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   text.h
 *
 *  \brief
 *    Interface file for textbox objects
 */

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

static constexpr size_t textSize = 12;

void init_font();

sf::Font &get_font();

const float &get_elipse_size();
