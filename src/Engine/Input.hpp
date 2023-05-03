#pragma once

#include <CoreGraphics/CGEvent.h>

namespace Temp
{
  namespace Input
  {
    CGEventRef OnKeystroke(
        CGEventTapProxy proxy,
        CGEventType type,
        CGEventRef event,
        void *data
    );
  }
}
