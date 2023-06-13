#pragma once

#include <vector>

namespace Game
{
  namespace Component
  {
    namespace Type
    {
      enum Type
      {
        VALUE = Temp::Component::Type::MAX,
        COLLECTED_VALUE,
        SCORE,
        MAX
      };
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
