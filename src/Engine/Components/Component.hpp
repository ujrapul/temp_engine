#pragma once

#include <cstdint>
#include <bitset>

namespace Temp
{
  constexpr std::uint8_t MAX_COMPONENTS = 32;

  // Used to track which components an entity has
  using Signature = std::bitset<MAX_COMPONENTS>;
}
