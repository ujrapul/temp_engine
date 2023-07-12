#pragma once

#include "Drawable.hpp"
#include "Hoverable.hpp"
#include "Luable.hpp"

namespace Temp::Component
{
  namespace Type
  {
    enum Type
    {
      POSITION2D = 0,
      DRAWABLE,
      SCALE,
      TEXT,
      HOVERABLE,
      LUABLE,
      MAX
    };
  }
  
  template <uint8_t> struct MapToComponentDataType_t;
  template <> struct MapToComponentDataType_t<Type::POSITION2D> { using type = Math::Vec2f; };
  template <> struct MapToComponentDataType_t<Type::DRAWABLE> { using type = Drawable::Data; };
  template <> struct MapToComponentDataType_t<Type::SCALE> { using type = float; };
  template <> struct MapToComponentDataType_t<Type::TEXT> { using type = std::string; };
  template <> struct MapToComponentDataType_t<Type::HOVERABLE> { using type = Hoverable::Data; };
  template <> struct MapToComponentDataType_t<Type::LUABLE> { using type = Luable::Data; };

  template <uint8_t T> using MapToComponentDataType = typename MapToComponentDataType_t<T>::type;
  
  template <uint8_t T> constexpr MapToComponentDataType<T> GetDefaultValue() { return {}; }
  template <> constexpr float GetDefaultValue<Type::SCALE>() { return 1.f; }
  
  // Used for Unit Tests
  template <uint8_t T> MapToComponentDataType<T> GetTestValue() { return {}; }
  template <> inline Math::Vec2f GetTestValue<Type::POSITION2D>() { return {10457349.f, -415470.f}; }
  template <> inline Drawable::Data GetTestValue<Type::DRAWABLE>()
  { return {{-34324, 234, 786}, {32, 234, 42}, {}, {}, 0, 1, 2, 3, 4, 5, 6}; }
  template <> inline float GetTestValue<Type::SCALE>() { return 10457349.f; }
  template <> inline std::string GetTestValue<Type::TEXT>() { return "gerbvrear"; }
  template <> inline Hoverable::Data GetTestValue<Type::HOVERABLE>()
  { return {nullptr, nullptr, nullptr, nullptr, nullptr, 234, 1523, 782, 981241}; }
}
