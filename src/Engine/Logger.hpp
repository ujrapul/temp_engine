#pragma once

#include <iostream>

namespace Temp::Logger
{
  enum class LogType
  {
    COUT,
    NOOP
  };
  
  inline LogType logType{LogType::COUT};
  
  struct LogInterface
  {
    virtual void Log(const char* text) const = 0;
    virtual void Log(const std::string& text) const = 0;
  };
  
  struct Noop : LogInterface
  {
    inline void Log(const char*) const override{}
    inline void Log(const std::string&) const override{}
  };

  struct Cout : LogInterface
  {
    inline void Log(const char* text) const override
    {
      std::cout << text << "\n";
    }
    
    inline void Log(const std::string& text) const override
    {
      Log(text.c_str());
    }
  };
    
  inline const LogInterface& GetLogger()
  {
    static const Cout cout{};
    static const Noop noop{};
    switch (logType)
    {
      case LogType::COUT:
        return cout;
      default:
        return noop;
    };
  }

  inline void Log(const char* text)
  {
    GetLogger().Log(text);
  }
  
  inline void Log(const std::string& text)
  {
    GetLogger().Log(text);
  }
}
