/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   barrier.h
 *
 *  \brief
 *    Interface file for barrier object
 */

#pragma once

#include <SFML/Graphics/Color.hpp>

#include "object.h"

/*!
 *  \class Barrier
 *
 *  \biref
 *    An object used to block a rays line of site with a given
 *    coefficent of reflection based on the material
 */
class Barrier : public Object
{
  public:
    enum COEFFICENTS
    {
      C_WOOD = 0
      , C_RUBBER
      , C_WALL

      , C_CUSTOM0
      , C_CUSTOM1
      , C_CUSTOM2
      , C_CUSTOM3
      , C_CUSTOM4
      , C_CUSTOM5
      , C_CUSTOM6
      , C_CUSTOM7
      , C_CUSTOM8
      , C_CUSTOM9

      , C_COUNT 
    };

    static inline const float INVALID_COEFFICENT_VALUE = -1.f;

    struct FrequencyCoefficents
    {
      static inline constexpr uint8_t VALUE_COUNT = 6;
      struct FreqCoeffValues
      {
        uint16_t frequency;
        float coefficent;
      } values[6];
      sf::Color color;
      std::string name;
    };

    struct Coefficents 
    {
      float coefficent;
      sf::Color color;
      std::string name;
    };

    Barrier(const Vec2 &pos, const Vec2 &size, const std::string &type);
    ~Barrier();

    const COEFFICENTS &get_type_data();

    static void reset_custom_coefficents();
    static COEFFICENTS get_next_free_custom_coefficent();
    static void set_custom_coefficent(const COEFFICENTS &index
        , const Coefficents &value);
    static COEFFICENTS get_coefficent_index(const std::string &name);
    static float get_coefficent(const COEFFICENTS &index);

  private:
    COEFFICENTS type;

    static inline Coefficents CoefficentValues[C_COUNT] =
    {
      // Standard Coefficents
      {0.1f, sf::Color{186, 140, 99}, "wood"}
      , {0.05f, sf::Color{255, 192, 203}, "rubber"}
      , {0.3f, sf::Color{100, 100, 100}, "wall"}
      // Custom Coefficents
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
      , {INVALID_COEFFICENT_VALUE, sf::Color{0, 0, 0}, ""}
    };

};
