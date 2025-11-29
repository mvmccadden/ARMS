/*!
 *  \author Manoel McCadden
 *  \date   11-27-25
 *  \file   helper.h
 *
 *  \brief
 *    Stores helper classes for processes such as logging
 */

#pragma once

#include <iostream>
#include <string>

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
