#pragma once

#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <type_traits>

namespace Temp
{
  using Entity = std::uint32_t;

  constexpr Entity MAX_ENTITIES = 5000;
}
