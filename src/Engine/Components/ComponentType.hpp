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
        SCALE,
        TEXT,
        MAX
      };
    }
    
    template <uint8_t> struct MapToComponentDataType_t;
    template <> struct MapToComponentDataType_t<Type::POSITION2D> { using type = Math::Vec2f; };
    template <> struct MapToComponentDataType_t<Type::DRAWABLE> { using type = Drawable::Data; };
    template <> struct MapToComponentDataType_t<Type::SCALE> { using type = float; };
    template <> struct MapToComponentDataType_t<Type::TEXT> { using type = std::string; };
    
    template <uint8_t T> using MapToComponentDataType = typename MapToComponentDataType_t<T>::type;

    template <uint8_t T> constexpr MapToComponentDataType<T> GetDefaultValue() { return {}; }
    template <> constexpr float GetDefaultValue<Type::SCALE>() { return 1.f; }
  }
}

