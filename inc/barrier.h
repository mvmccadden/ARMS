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

#include "helper.h"
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

    // EMPTY CARRAY OF VEC2
    static inline const CArray<Vec2> INVALID_COEFFICENT_VALUE;

    struct EQCoefficents
    {
      CArray<Vec2> frequencyCoefficents;
      sf::Color color;
      std::string name;
    };

    Barrier(const Vec2 &pos, const Vec2 &size, const std::string &type);
    ~Barrier();

    const COEFFICENTS &get_type_data();

    static void reset_custom_coefficents();
    static COEFFICENTS get_next_free_custom_coefficent();
    static void set_custom_coefficent(const COEFFICENTS &index
        , const EQCoefficents &value);
    static COEFFICENTS get_coefficent_index(const std::string &name);
    static const CArray<Vec2> &get_coefficent(const COEFFICENTS &index);

  private:
    COEFFICENTS type;

    static inline EQCoefficents EQCoefficentValues[C_COUNT] =
    {
      // Standard Coefficents
      {CArray<Vec2>{{125, 0.28f}, {500, 0.17f}, {2000, 0.1f}, {4000, 0.15f}}
        , sf::Color{186, 140, 99}, "wood"}
      , {CArray<Vec2>{{125, 0.04}, {500, 0.06}, {2000, 0.1f}, {4000, 0.15}}
        , sf::Color{255, 192, 203}, "rubber"}
      , {CArray<Vec2>{{125, 0.18}, {500, 0.04}, {2000, 0.03f}, {4000, 0.02}}
        , sf::Color{100, 100, 100}, "wall"}
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
