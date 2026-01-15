/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   audioray.cpp
 *
 *  \brief
 *    Implementation of audio ray object 
 */

#include "barrier.h"
#include "helper.h"
#include <cstring>

using namespace std;

Barrier::Barrier(const Vec2 &pos, const Vec2 &size, const string &_type)
  : Object(pos, size, "Barrier")
{
  static_cast<void>(Logger(Logger::L_MSG
        , "Creating new barrier of type: " + _type));

  for(size_t i = 0; i < C_COUNT; ++i)
  {
    if(_type == EQCoefficentValues[i].name)
    {
      set_color(EQCoefficentValues[i].color);
      // Resize so that extra indicies arn't made
      absortionCoefficents.resize(
          EQCoefficentValues[i].frequencyCoefficents.size());
      // Populate the indicies
      for(size_t j = 0; j < EQCoefficentValues[i].frequencyCoefficents.size();
          ++j)
      {
        absortionCoefficents[j] = EQCoefficentValues[i].frequencyCoefficents[j];
        Logger(Logger::L_MSG
            , "Absorbtion Coefficent " 
            + to_string(absortionCoefficents[j].x) 
            + ": " + to_string(absortionCoefficents[j].y));
      }
      type = static_cast<COEFFICENTS>(i);
      return;
    }
  }
  
  set_color(sf::Color::Black);
  absortionCoefficents = INVALID_COEFFICENT_VALUE;
  type = C_COUNT;
}

Barrier::~Barrier() { }

const Barrier::COEFFICENTS &Barrier::get_type_data()
{
  return type;
}

void Barrier::reset_custom_coefficents()
{
  for(size_t i = C_CUSTOM0; i < C_COUNT; ++i)
  {
    EQCoefficentValues[i].frequencyCoefficents = INVALID_COEFFICENT_VALUE;
    EQCoefficentValues[i].name = "";
    EQCoefficentValues[i].color = sf::Color::Black;
  }
}

Barrier::COEFFICENTS Barrier::get_next_free_custom_coefficent()
{
  for(size_t i = C_CUSTOM0; i < C_COUNT; ++i)
  {
    // INVALID COEFFICENT VALUE is an empty CArray<Vec2>
    // It also has no name
    if(EQCoefficentValues[i].name == "")
    {
      static_cast<void>(Logger(Logger::L_MSG
            , "Found next free custom coefficent"));
      return static_cast<COEFFICENTS>(i);
    }
  }

  return C_COUNT;
}

void Barrier::set_custom_coefficent(const COEFFICENTS &index
    , const EQCoefficents &value)
{
  if(index == C_COUNT)
  {
    static_cast<void>(Logger(Logger::L_MSG
          , "Invalid index based for custom coefficent"));
    return;
  }

  EQCoefficentValues[index].name = value.name;
  EQCoefficentValues[index].color = value.color;
  EQCoefficentValues[index].frequencyCoefficents = value.frequencyCoefficents;
}

Barrier::COEFFICENTS Barrier::get_coefficent_index(const string &name)
{
  for(size_t i = 0; i < C_COUNT; ++i)
  {
    if(EQCoefficentValues[i].name == name)
    {
      return static_cast<COEFFICENTS>(i);
    }
  }

  return C_COUNT;
}

const CArray<Vec2> &Barrier::get_coefficent(const COEFFICENTS &index)
{
  if(index == C_COUNT)
  {
    static_cast<void>(Logger(Logger::L_MSG
          , static_cast<string>("Invalid index based for get coefficent,")
          + " return invalid cofficent"));
    return INVALID_COEFFICENT_VALUE;
  }

  return EQCoefficentValues[index].frequencyCoefficents;
}
