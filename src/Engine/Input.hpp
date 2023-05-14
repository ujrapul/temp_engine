#pragma once

#ifdef __APPLE__
#include <CoreGraphics/CGEvent.h>
#endif

#include <string>
#include <array>
#include <vector>
#include <mutex>

namespace Temp
{
  namespace Input
  {
    struct KeyEventData
    {
      std::array<std::vector<void (*)()>, 128> keyEvents;
      std::mutex lock;
    };

#ifdef __APPLE__
    // The following method converts the key code returned by each keypress as
    // a human readable key code in const char format.
    std::string convertKeyCode(int keyCode, bool shift, bool caps);
    CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* data);
#endif

    void Handle(KeyEventData& data);
    
    void AddCallback(void (*Callback)(), KeyEventData& data, int keyCode);
    void RemoveCallback(void (*Callback)(), KeyEventData& data, int keyCode);
  }
}
