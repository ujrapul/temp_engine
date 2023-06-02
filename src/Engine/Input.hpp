#pragma once

#ifdef __APPLE__
#include <CoreGraphics/CGEvent.h>
#elif _WIN32
#include <winuser.h>
#endif

#include <string>
#include <array>
#include <vector>
#include <mutex>
#include <queue>

namespace Temp
{
  namespace Input
  {
    enum class KeyboardCode : uint8_t
    {
#ifdef __linux__
      KB_0 = 11,
      KB_1 = 2,
      KB_2 = 3,
      KB_3 = 4,
      KB_4 = 5,
      KB_5 = 6,
      KB_6 = 7,
      KB_7 = 8,
      KB_8 = 9,
      KB_9 = 10,
      KB_Q = 16,
#elif __APPLE_
      KB_0 = 29,
      KB_1 = 18,
      KB_2 = 19,
      KB_3 = 20,
      KB_4 = 21,
      KB_5 = 23,
      KB_6 = 22,
      KB_7 = 26,
      KB_8 = 28,
      KB_9 = 25,
      KB_Q = 12,
#elif _WIN32
      KB_0 = VK_0,
      KB_1 = VK_1,
      KB_2 = VK_2,
      KB_3 = VK_3,
      KB_4 = VK_4,
      KB_5 = VK_5,
      KB_6 = VK_6,
      KB_7 = VK_7,
      KB_8 = VK_8,
      KB_9 = VK_9,
      KB_Q = VK_Q,
#endif
      KB_MAX = 255
    };

    struct KeyQueue;
    
    struct KeyEventData
    {
      std::array<std::vector<void (*)(KeyboardCode)>, 128> keyEvents;
      KeyQueue* keyQueue;
    };

    std::string convertKeyCode(KeyboardCode keyCode, bool shift, bool caps);

#ifdef __APPLE__
    // The following method converts the key code returned by each keypress as
    // a human readable key code in const char format.
    CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* data);
#endif
    
    [[nodiscard]] KeyEventData Construct();
    void Destruct(KeyEventData& data);

    void HandleThread(KeyEventData& data);
    void Process(KeyEventData& data);
    
#ifdef __linux__
    void PushKeyQueue(KeyboardCode keyCode, KeyEventData *data);
#endif

    void AddCallback(void (*Callback)(KeyboardCode), KeyEventData& data, KeyboardCode keyCode);
    void RemoveCallback(void (*Callback)(KeyboardCode), KeyEventData& data, KeyboardCode keyCode);
  }
}
