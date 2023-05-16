#pragma once

namespace Temp
{
  namespace Component
  {
    namespace Type
    {
      const uint8_t POSITION2D = 0;
      const uint8_t MAX = 1;
    }
    
    template <uint8_t> struct MapToComponentDataType_t;
    template <> struct MapToComponentDataType_t<Type::POSITION2D> { using type = Math::Vec2; };
    
    template <uint8_t T>
    using MapToComponentDataType = typename MapToComponentDataType_t<T>::type;
  }
}

