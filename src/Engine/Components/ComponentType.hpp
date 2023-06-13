#pragma once

#include "Drawable.hpp"

namespace Temp
{
  namespace Component
  {
    namespace Type
    {
      enum Type
      {
        POSITION2D = 0,
        DRAWABLE,
        MAX
      };
    }
    
    template <uint8_t> struct MapToComponentDataType_t;
    template <> struct MapToComponentDataType_t<Type::POSITION2D> { using type = Math::Vec2f; };
    template <> struct MapToComponentDataType_t<Type::DRAWABLE> { using type = Drawable::Data; };
    
    template <uint8_t T>
    using MapToComponentDataType = typename MapToComponentDataType_t<T>::type;
  }
}

