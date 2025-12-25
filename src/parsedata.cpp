/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   parsedata.h
 *
 *  \brief
 *    Implementation file for parsing data 
 */

#include "parsedata.h"

#include <fstream>

#include "barrier.h"

#include "arms_math.h"
#include "helper.h"

using namespace std;

int get_int_from_line(const string &line, const size_t &startingPos)
{
  int digit = 0;
  for(size_t i = startingPos; i < line.size(); ++i)
  {
    char lineChar = line[i];
    if(lineChar >= '0' && lineChar <= '9')
    {
      digit *= 10;
      digit += static_cast<int>(lineChar) - 48;
    }
  }

  return digit;
}

float get_double_from_line(const string &line, const size_t &startingPos)
{
  float decimal = 0.f;
  uint8_t decimalPlaces = 0;
  bool rhs = false;
  for(size_t i = startingPos; i < line.size(); ++i)
  {
    char lineChar = line[i];
    if(lineChar >= '0' && lineChar <= '9')
    {
      if(!rhs)
      {
        decimal *= 10.f;
        decimal += static_cast<int>(lineChar) - 48;
      }
      else
      {
        float value = static_cast<int>(lineChar) - 48;
        ++decimalPlaces;

        for(uint8_t i = 0; i < decimalPlaces; ++i)
        {
          value *= 0.1f;
        }

        decimal += value;
      }
    }
    // ascii 46 = '.'
    else if(lineChar == 46)
    {
      rhs = true;
    }
  }
  
  return decimal;
}

/*!
 *  Reads a file from the documents directory with a given name
 *
 *  \param fileName
 *    The file being read
 *
 *  \returns 
 *    A pointer to the top of the data map
 */
DataMap *read_scene_file(string fileName, const bool &ignoreInputDir)
{
  ifstream file;
  if(ignoreInputDir)
  {
    file.open(fileName);
  }
  else
  {
    file.open(INPUT_DIR + fileName + ".txt");
  }

  if(!file)
  {
    static_cast<void>(Logger(Logger::L_ERR
          , "Failed to access given test file"));
    return nullptr;
  }

  // Reset barrier custom settings to ensure they don't get filled
  Barrier::reset_custom_coefficents();
    
  // Creating first node as root.
  DataMap *dataMap = new DataMap("root", nullptr);

  string line;
  while(getline(file, line))
  {
    if(line.find("Source") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Source", dataMap));
    }
    else if(line.find("Listener") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Listener", dataMap));
    }
    else if(line.find("Barrier") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Barrier", dataMap));
    }
    else if(line.find("Position") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Position", dataMap));
    }
    else if(line.find("Size") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Size", dataMap));
    }
    else if(line.find("Color") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Color", dataMap));
    }
    else if(line.find("Direction") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Direction", dataMap));
    }
    else if(line.find("Cone") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Cone", dataMap));
    }
    else if(line.find("Checks") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Checks", dataMap));
    }
    else if(line.find("Rays") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Rays", dataMap));
    }
    else if(line.find("Pattern") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Pattern", dataMap));
    }
    else if(line.find("Coefficent") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Coefficent", dataMap));
    }
    else if(line.find("Array[") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Array", dataMap));
      // Create a C-style array of a user defined number of digits
      int size = get_int_from_line(line, line.find("Array[") + sizeof("Array["));
      // Detects if a char was included to denote type of array objects, default
      // is "I" or int
      if(line.find("D"))
      {
        dataMap->set_data(new CQueue<float>(size));
      }
      else if(line.find("S"))
      {
        dataMap->set_data(new CQueue<string>(size));
      }
      else
      {
        dataMap->set_data(new CQueue<int>(size));
      }
    }
    else if(line.find("}") != string::npos)
    {
      dataMap = dataMap->get_parent();
    }
    else if(line.find("Int =") != string::npos)
    {
      int digit = get_int_from_line(line, line.find("Int =") + sizeof("Int ="));
      // Handle arrays with a queue
      if(dataMap->get_name() == "Array")
      {
        dataMap->get_casted_data<CQueue<int>>()->push(digit);
      }
      else
      {
        dataMap->set_data(new int(digit));
      }
    }
    // TODO: FIX DOUBLE, STRING, VEC2, VEC3 PARSING LIKE I DID WITH INT
    else if(line.find("Double =") != string::npos)
    {
      float decimal = get_double_from_line(line, sizeof("Double ="));
      // Handle arrays with a queue
      if(dataMap->get_name() == "Array")
      {
        dataMap->get_casted_data<CQueue<float>>()->push(decimal);
      }
      else
      {
        dataMap->set_data(new float(decimal));
      }
    }
    else if(line.find("String =") != string::npos)
    {
      string str;
      bool rhs = false;
      for(char lineChar : line)
      {
        if(lineChar == '=')
        {
          rhs = true;
        }

        if(!rhs || lineChar == ' ')
        {
          continue;
        }

        if(lineChar >= 65 && lineChar <= 90)
        {
          lineChar += 32;
        }

        if(lineChar >= 97 && lineChar <= 122)
        {
          str += lineChar;
        }
      }

      dataMap->set_data(new string(str));
    }
    else if(line.find("Vec2 =") != string::npos)
    {
      array<int, 2> digits = {0};
      int currentDigit = 0;
      bool rhs = false;
      for(char lineChar : line)
      {
        if(lineChar == '=')
        {
          rhs = true;
        }

        // Is a digit ascii char
        else if(rhs && lineChar >= '0' && lineChar <= '9')
        {
          // 48 = 0
          digits[currentDigit] *= 10;
          digits[currentDigit] += static_cast<int>(lineChar) - 48;
        }
        else if(rhs && lineChar == ',')
        {
          ++currentDigit;
        }
      }

      dataMap->set_data<Vec2>(new Vec2(static_cast<float>(digits[0])
            , static_cast<float>(digits[1])));
    }
    else if(line.find("Vec3 =") != string::npos)
    {
      array<uint8_t, 3> digits = {0};
      int currentDigit = 0;
      bool rhs = false;
      for(char lineChar : line)
      {
        if(lineChar == '=')
        {
          rhs = true;
        }

        // Is a digit ascii char
        else if(rhs && lineChar >= '0' && lineChar <= '9')
        {
          // 48 = 0
          digits[currentDigit] *= 10;
          digits[currentDigit] += static_cast<int>(lineChar) - 48;
        }
        else if(rhs && lineChar == ',')
        {
          ++currentDigit;
        }
      }

      dataMap->set_data<Vec3>(new Vec3(static_cast<float>(digits[0])
            , static_cast<float>(digits[1]), static_cast<float>(digits[2])));
    }
  }

  return dataMap;
}
