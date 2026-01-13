/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   helper.h
 *
 *  \brief
 *    Stores helper classes for processes such as logging
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <memory.h>

class Logger
{
  public:
    enum L_SEVERITY
    {
      L_DEFAULT = 0
      , L_MSG
      , L_WRN
      , L_ERR
    };

    Logger(const L_SEVERITY &severity, const std::string &message)
      : logSeverity(severity), logMessage(message)
    {
      if(globalSevertiy <= severity)
      {
        std::cout << message << std::endl;
      }
    }
    ~Logger() {}

    static void SetGlobalSevertiy(const L_SEVERITY &severity)
    {
      globalSevertiy = severity;
    }

  private:
    static inline L_SEVERITY globalSevertiy = L_DEFAULT;

    L_SEVERITY logSeverity;
    std::string logMessage;
};

template <typename T>
class CArray
{
  public:
    CArray()
      : head(nullptr), count(0)
    {
    }

    CArray(const size_t &initalcount)
      : head(new T[initalcount]{}), count(initalcount)
    {
    }

    CArray(const CArray &other)
      : head(nullptr), count(0)
    {
      *this = other;
    }

    /*!
     *  Creates a C-Style array with a std::initializer_list
     *
     *  \param array
     *    An array of type T with a size of N values
     */
    CArray(std::initializer_list<T> array)
      : head(new T[array.size()]{}), count(array.size())
    {
      size_t i = 0;
      for(const T &value : array)
      {
        head[i++] = value;
      }
    }

    ~CArray()
    {
      clear();
    }

    CArray &operator=(const CArray &other)
    {
      if(this == &other)
      {
        return *this;
      }

      clear();

      if(other.count == 0)
      {
        return *this;
      }

      resize(other.count);

      for(size_t i = 0; i < count; ++i)
      {
        head[i] = other.head[i];
      }

      return *this;
    }


    CArray &operator+=(const CArray &other)
    {
      if(count != other.count)
      {
        static_cast<void>(Logger(Logger::L_MSG
              , "CArray sizes do not match for += operation"));

        if(count < other.count)
        {
          resize(other.count);
        }
      }
      
      // Add up to other count since this head will be resized if it is smaller
      // than the others head
      for(size_t i = 0; i < other.count; ++i)
      {
        head[i] += other.head[i];
      }

      return *this;
    }

    CArray operator+(const CArray &other)
    {
      CArray output(*this);
      return output += other;
    }


    const size_t &size() const
    {
      return count;
    }

    const T *front() const
    {
      return head;
    }
  
    T &operator[](const size_t &index)
    {
      if(index >= count)
      {
        /*
        // Send a warning if attempting to dereference but resize the array
        static_cast<void>(Logger(Logger::L_WRN, 
              std::string("Attempted to dereference an invalid index value. ")
              + "Instead, reallocated sample array to size of: " 
              + std::to_string((index + 1) * 2)));
        */
        resize((index + 1) * 2);
      }

      return head[index];
    }

    const T &at(const size_t &index) const
    {
      if(!head || count == 0)
      {
        throw "Calling at() when no valid value in CArray";
      }

      if(index >= count)
      {
        /*
        static_cast<void>(Logger(Logger::L_WRN, 
              std::string("Attempted to dereference an invalid index value. ")
              + "Instead, returning last index value!"));
        */
        return head[count - 1];
      }

      return head[index];
    }


    void resize()
    {
      resize(count * 2);
    }

    void resize(const size_t &newSize)
    {
      if(newSize == 0)
      {
        clear();
        return;
      }

      T *newHead = new T[newSize]{};

      size_t copyCount = (count < newSize) ? count : newSize;

      if(head && count > 0)
      {
        for(size_t i = 0; i < copyCount; ++i)
        {
          newHead[i] = head[i];
        }
      }

      clear();

      head = newHead;
      count = newSize;
    }

    void clear()
    {
      if(head != nullptr)
      {
        /*
        std::stringstream ss; 
        ss << static_cast<const void *>(head);
        static_cast<void>(Logger(Logger::L_MSG, "CLEARING CARRAY. SIZE: "
            + std::to_string(count) + ", PTR: " + ss.str()));
        */
        delete []head;
        head = nullptr;
      }

      count = 0;
    }

  private:
    size_t count;
    T *head;
};

// FIFO
template<typename T>
class CQueue
{
  public:
    CQueue()
      : front_(0), back_(0), count_(0), array()
    {
    }

    CQueue(const size_t &size)
      : front_(0), back_(0), count_(0), array(size)
    {
    }

    CQueue(const CQueue &other)
      : front_(0), back_(0), count_(0), array()
    {
      *this = other;
    }

    ~CQueue()
    {
      clear();
    }

    CQueue &operator=(const CQueue &other)
    {
      if(this == &other)
      {
        return *this;
      }

      array = other.array;
      front_ = other.front_;
      back_ = other.back_;

      return *this;
    }


    const T &front() const
    {
      return array[front_];
    }

    const T &back() const
    {
      return array[back_];
    }

    size_t size() const
    {
      return count_;
    }

  
    void push(const T &value)
    {
      if(count_ == array.size())
      {
        resize();
      }

      array[back_] = value;
      back_ = (back_ + 1) % ((array.size() == 0) ? 1 : array.size());
      ++count_;
    }

    /*!
     *  Pops the first index in the queue
     *
     *  \returns
     *    The popped index if it exists
     */
    T pop()
    {
      if(count_ == 0)
      {
        throw "Invalid pop on empty CQueue";
      }

      size_t currentFront = front_;
      front_ = (front_ + 1) % array.size();
      --count_;

      return array[currentFront];
    }


    void clear()
    {
      array.clear();
      front_ = 0;
      back_ = 0;
    }

  private:
    /*!
     *  Realligns and resizes the queue by moving the front to index 0 and the
     *  back to the previous size, while doubling the queue's size
     */
    void resize()
    {
      size_t size = array.size();
      CArray<T> newArray((size == 0) ? 1 : size * 2);

      for(size_t i = 0; i < count_; ++i)
      {
        newArray[i] = array[(front_ + i) % size];
      }

      array = newArray;
      front_ = 0;
      back_ = count_;
    }

    size_t front_ = 0, back_ = 0, count_ = 0;
    CArray<T> array;
};
