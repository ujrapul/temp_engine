#pragma once

#include <cassert>
#include <iostream>

namespace Temp
{
  struct MockComponent
  {
    std::string name{""};
    int value{};
    bool toggle{false};
    
    bool operator==(const MockComponent& other) const = default;
  };

  inline void Assert(const char* testName, bool value)
  {
    if (!value)
    {
      std::cout << "FAILED " << testName << std::endl;
    }
    assert(value);
  }
}
