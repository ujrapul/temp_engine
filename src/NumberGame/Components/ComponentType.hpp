#pragma once

#include <vector>

namespace Game
{
  namespace Component
  {
    namespace Type
    {
      constexpr uint8_t VALUE             = Temp::Component::Type::MAX;
      constexpr uint8_t COLLECTED_VALUE   = Temp::Component::Type::MAX + 1;
      constexpr uint8_t SCORE             = Temp::Component::Type::MAX + 2;
    }
  }
}

namespace Temp
{
  namespace Component
  {
    using namespace Game::Component::Type;
    
    template <> struct MapToComponentDataType_t<VALUE> { using type = char; };
    template <> struct MapToComponentDataType_t<COLLECTED_VALUE> {
      using type = std::vector<MapToComponentDataType_t<VALUE>::type>;
    };
    template <> struct MapToComponentDataType_t<SCORE> { using type = int; };
  }
}
