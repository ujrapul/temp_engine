#pragma once

namespace Temp
{
  namespace Component
  {
    enum class Type : std::uint8_t
    {
      POSITION2D = 0,
      MAX = 1,
    };
    
    template <Type> struct MapToComponentDataType_t;
    template <> struct MapToComponentDataType_t<Type::POSITION2D> { using type = Math::Vec2; };
    
    template <Type T>
    using MapToComponentDataType = typename MapToComponentDataType_t<T>::type;
  }
}

