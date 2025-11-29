/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   parsedata.h
 *
 *  \brief
 *    Interface file for parsing data 
 */

#pragma once

#include <string>
#include <typeinfo>
#include <functional>
#include <unordered_map>

#include "helper.h"

class Object;
class AudioRay;

class DataMap
{
  public:
    using DataMapIterator = 
      std::vector<DataMap *>::iterator;
    using DataDtor = std::function<void(void *)>;

    DataMap(const std::string &_name, DataMap *_parent)
      : name(_name), parent(_parent)
    {
    }
    ~DataMap()
    {
      for(DataMap *data : children)
      {
        delete data;
      }
      free_data();
    }

    std::string get_name()
    {
      return name;
    }
    
    template<typename T>
    void set_data(T *_data)
    {
      if(data != nullptr)
      {
        free_data();
      }

      data = static_cast<void*>(_data);
      dataDtor = 
        [](void *d) 
        {
          delete static_cast<T *>(d);
        };
    }

    const std::type_info &get_data_type() const
    {
      return typeid(DataDtor);
    }

    template<typename T>
    T *get_casted_data()
    {
      return static_cast<T *>(data);
    }

    void *get_raw_data()
    {
      return data;
    }

    DataMap *add_child(DataMap *child)
    {
      children.push_back(child);
      return (child);
    }

    DataMap *get_parent()
    {
      return parent;
    }

    DataMapIterator get_children_begin()
    {
      return children.begin();
    }

    DataMapIterator get_children_end()
    {
      return children.end();
    }

    void free_data()
    {
      dataDtor(data);
    }

  private:
    std::string name = "";
    DataMap *parent = nullptr;
    void *data = nullptr;
    DataDtor dataDtor;
    std::vector<DataMap *> children;
};

struct RayGenerationInfo
{
  int rayCount = 10;
  int maxChecks = 10;
  // In degrees
  int coneSize = 0;
  // In Degrees (center of cone)
  int direction = 0;
};

DataMap *read_scene_file(std::string fileName
    , const bool &ignoreInputDir = false);
