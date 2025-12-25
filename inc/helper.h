/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   helper.h
 *
 *  \brief
 *    Stores helper classes for processes such as logging
 */

#pragma once

#include <cstddef>
#include <iostream>
#include <string>
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
      : head(new T[initalcount]), count(initalcount)
    {
    }

    CArray(const CArray &other)
      : head(nullptr), count(0)
    {
      *this = other;
    }

    ~CArray()
    {
      clear();
    }

    CArray &operator=(const CArray &other)
    {
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
        // Send a warning if attempting to dereference but resize the array
        static_cast<void>(Logger(Logger::L_WRN, 
              std::string("Attempted to dereference an invalid index value. ")
              + "Instead, reallocated sample array to size of: " 
              + std::to_string(index + 1)));
        resize(index + 1);
      }

      return head[index];
    }

    const T &at(const size_t &index) const
    {
      if(index >= count)
      {
        static_cast<void>(Logger(Logger::L_WRN, 
              std::string("Attempted to dereference an invalid index value. ")
              + "Instead, returning last index value!"));
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

      T *newHead = new T[newSize];

      size_t copyCount = (count < newSize) ? count : newSize;

      if(count > 0)
      {
        for(size_t i = 0; i < copyCount; ++i)
        {
          newHead[i] = head[i];
        }
      }
      // Set all newly allocated index's to 0 if the size is now greater than
      // before
      if(newSize > count)
      {
        memset(newHead + copyCount, 0, (newSize - copyCount) * sizeof(T));
      }

      clear();

      head = newHead;
      count = newSize;
    }

    void clear()
    {
      if(head)
      {
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
      back_ = (back_ + 1) % array.size();
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
        return T();
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
