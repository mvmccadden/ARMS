/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   barrier.h
 *
 *  \brief
 *    Interface file for barrier object
 */

#pragma once

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
    Barrier(const Vec2 &pos, const Vec2 &size, const std::string &type);
    ~Barrier();

    const std::string &get_type_data();

  private:
    const std::string type;
};
