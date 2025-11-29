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

#include "arms_math.h"

using namespace std;

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
    cout << "Failed to access given text file" << endl;
    return nullptr;
  }
    
  // Creating first node as root.
  DataMap *dataMap = new DataMap("root", nullptr);

  string line;
  while(getline(file, line))
  {
    if(line.find("Info") != string::npos)
    {
      dataMap = dataMap->add_child(new DataMap("Info", dataMap));
      dataMap->set_data<RayGenerationInfo>(new RayGenerationInfo());
    }
    else if(line.find("Source") != string::npos)
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
    else if(line.find("}") != string::npos)
    {
      dataMap = dataMap->get_parent();
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
    else if(line.find("Type =") != string::npos)
    {
      string type;
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
          type += lineChar;
        }
      }

      dataMap->set_data<string>(new string(type));
    }
    else if(line.find("raycount =") != string::npos)
    {
      int digit = 0;
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
          digit *= 10;
          digit += static_cast<int>(lineChar) - 48;
        }
      }

      RayGenerationInfo *info = dataMap->get_casted_data<RayGenerationInfo>();
      info->rayCount = digit;
    }
    else if(line.find("maxchecks =") != string::npos)
    {
      int digit = 0;
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
          digit *= 10;
          digit += static_cast<int>(lineChar) - 48;
        }
      }

      RayGenerationInfo *info = dataMap->get_casted_data<RayGenerationInfo>();
      info->maxChecks = digit;
    }
    else if(line.find("conesize =") != string::npos)
    {
      int digit = 0;
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
          digit *= 10;
          digit += static_cast<int>(lineChar) - 48;
        }
      }

      RayGenerationInfo *info = dataMap->get_casted_data<RayGenerationInfo>();
      info->coneSize = digit;
    }
    else if(line.find("direction =") != string::npos)
    {
      int digit = 0;
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
          digit *= 10;
          digit += static_cast<int>(lineChar) - 48;
        }
      }

      RayGenerationInfo *info = dataMap->get_casted_data<RayGenerationInfo>();
      info->direction = digit;
    }
    else if(line.find("Pattern =") != string::npos)
    {
      string type;
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
          type += lineChar;
        }
      }

      dataMap->set_data<string>(new string(type));
    }
  }

  return dataMap;
}
