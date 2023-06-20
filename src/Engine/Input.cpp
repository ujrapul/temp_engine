#include "Input.hpp"
#include <iostream>
#include <algorithm>
#ifdef __linux__
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/input.h>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

// #define test_bit(bit, array)    ((array)[(bit)/8] & (1 << ((bit)%8)))

namespace
{
#ifdef __APPLE__
  CGEventFlags lastFlags = 0;

  constexpr void MacInputHandle(Temp::Input::KeyEventData &data)
  {
    // Create an event tap to retrieve keypresses.
    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap,
                                              kCGHeadInsertEventTap,
                                              kCGEventTapOptionDefault,
                                              eventMask, Temp::Input::CGEventCallback, std::addressof(data));

    // Exit the program if unable to create the event tap.
    if (!eventTap)
    {
      std::cerr << "ERROR: Unable to create event tap.\n";
      return;
    }

    // Create a run loop source and add enable the event tap.
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    CFRunLoopRun();
    
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();
    CFRunLoopRemoveSource(runLoop, runLoopSource, kCFRunLoopDefaultMode);

    // Step 2: Invalidate the run loop source
    CFRunLoopSourceInvalidate(runLoopSource);

    // Step 3: Release the run loop source
    CFRelease(runLoopSource);
  }
#elif __linux__

  std::vector<std::string> GetDevices()
  {
    const char *inputDeviceDir = "/dev/input";
    std::vector<std::string> devices;

    int fdIndex = 0;
    DIR *dir = opendir(inputDeviceDir);
    if (!dir)
    {
      std::cerr << "Failed to open input device directory: " << inputDeviceDir << std::endl;
      return {};
    }

    std::string keyboardPath;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
      if (entry->d_name[0] != '.')
      {
        if (std::string(entry->d_name) != "by-id" && std::string(entry->d_name) != "by-path") {
          devices.push_back(std::string(inputDeviceDir) + "/" + entry->d_name);
        }
        // fd[fdIndex++] = open(devicePath.c_str(), O_RDONLY | O_NONBLOCK);
      }
    }

    closedir(dir);
    return devices;
  }

#endif

  constexpr void ActivateCallBack(Temp::Input::KeyboardCode keyCode, Temp::Input::KeyEventData *data)
  {
    for (auto fn : data->keyEvents[static_cast<int>(keyCode)])
    {
      fn(keyCode);
    }
  }
}

namespace Temp::Input
{
  struct KeyQueue
  {
    std::queue<KeyboardCode> queue;
    std::mutex lock;
  };

  void PushKeyQueue(KeyboardCode keyCode, KeyEventData *data)
  {
    std::scoped_lock<std::mutex> lock(data->keyQueue->lock);
    data->keyQueue->queue.push(keyCode);
  }

  KeyboardCode PopKeyQueue(KeyEventData *data)
  {
    std::scoped_lock<std::mutex> lock(data->keyQueue->lock);
    if (data->keyQueue->queue.size() == 0)
    {
      return KeyboardCode::KB_MAX;
    }
    KeyboardCode keyCode = data->keyQueue->queue.front();
    data->keyQueue->queue.pop();
    return keyCode;
  }

  // USING AS REFERENCE
  // The following method converts the key code returned by each keypress as
  // a human readable key code in const char format.
  std::string convertKeyCode(KeyboardCode keyCode, bool shift, bool caps)
  {
    switch (keyCode)
    {
    case KeyboardCode::KB_Q:
      return shift || caps ? "Q" : "q";
    case KeyboardCode::KB_0:
      return shift ? ")" : "0";
    case KeyboardCode::KB_1:
      return shift ? "!" : "1";
    case KeyboardCode::KB_2:
      return shift ? "@" : "2";
    case KeyboardCode::KB_3:
      return shift ? "#" : "3";
    case KeyboardCode::KB_4:
      return shift ? "$" : "4";
    case KeyboardCode::KB_5:
      return shift ? "%" : "5";
    case KeyboardCode::KB_6:
      return shift ? "^" : "6";
    case KeyboardCode::KB_7:
      return shift ? "&" : "7";
    case KeyboardCode::KB_8:
      return shift ? "*" : "8";
    case KeyboardCode::KB_9:
      return shift ? "(" : "9";
    default:
      break;
    }
    return "[unknown]";
  }

#ifdef __APPLE__
  CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *data)
  {
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged)
    {
      return event;
    }

    CGEventFlags flags = CGEventGetFlags(event);

    // Retrieve the incoming keycode.
    CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    // Calculate key up/down.
    bool down = false;
    if (type == kCGEventFlagsChanged)
    {
      switch (keyCode)
      {
      case 54: // [right-cmd]
      case 55: // [left-cmd]
        down = (flags & kCGEventFlagMaskCommand) && !(lastFlags & kCGEventFlagMaskCommand);
        break;
      case 56: // [left-shift]
      case 60: // [right-shift]
        down = (flags & kCGEventFlagMaskShift) && !(lastFlags & kCGEventFlagMaskShift);
        break;
      case 58: // [left-option]
      case 61: // [right-option]
        down = (flags & kCGEventFlagMaskAlternate) && !(lastFlags & kCGEventFlagMaskAlternate);
        break;
      case 59: // [left-ctrl]
      case 62: // [right-ctrl]
        down = (flags & kCGEventFlagMaskControl) && !(lastFlags & kCGEventFlagMaskControl);
        break;
      case 57: // [caps]
        down = (flags & kCGEventFlagMaskAlphaShift) && !(lastFlags & kCGEventFlagMaskAlphaShift);
        break;
      default:
        break;
      }
    }
    else if (type == kCGEventKeyDown)
    {
      down = true;
    }
    lastFlags = flags;

    // Only log key down events.
    if (!down)
    {
      return event;
    }

    // Print the human readable key to the logfile.
//    bool shift = flags & kCGEventFlagMaskShift;
//    bool caps = flags & kCGEventFlagMaskAlphaShift;

    //      std::cout << "Input received: " << convertKeyCode(keyCode, shift, caps) << std::endl;
    //      MacActivateCallBack(keyCode, static_cast<KeyEventData*>(data));
    PushKeyQueue(static_cast<KeyboardCode>(keyCode), static_cast<KeyEventData *>(data));

    return event;
  }
#endif

  KeyEventData Construct()
  {
    KeyEventData out;
    out.keyQueue = new KeyQueue();
    return out;
  }

  void Destruct(KeyEventData &data)
  {
    delete data.keyQueue;
  }

  void HandleThread(KeyEventData &data)
  {
#ifdef __APPLE__
    MacInputHandle(data);
#endif
  }

  void Process(KeyEventData &data)
  {
    KeyboardCode keyCode = PopKeyQueue(&data);
    if (keyCode != KeyboardCode::KB_MAX)
    {
      ActivateCallBack(keyCode, &data);
    }
  }

  void AddCallback(void (*FnPtr)(KeyboardCode), KeyEventData &data, KeyboardCode keyCode)
  {
    std::vector<void (*)(KeyboardCode)> &keyEvents = data.keyEvents[static_cast<int>(keyCode)];
    if (std::find(keyEvents.begin(), keyEvents.end(), FnPtr) != keyEvents.end())
    {
      return;
    }
    keyEvents.push_back(FnPtr);
  }

  void RemoveCallback(void (*FnPtr)(KeyboardCode), KeyEventData &data, KeyboardCode keyCode)
  {
    std::vector<void (*)(KeyboardCode)> &keyEvents = data.keyEvents[static_cast<int>(keyCode)];
    auto iterator = std::find(keyEvents.begin(), keyEvents.end(), FnPtr);
    if (iterator != keyEvents.end())
    {
      keyEvents.erase(iterator);
    }
  }
}
