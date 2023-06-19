#pragma once

#include "ComponentData.hpp"
#include "ComponentType.hpp"

namespace Temp::Component::Container
{
  constexpr uint8_t MAX = 128;

  struct Data
  {
    std::array<void *, MAX> components;
  };

  template <uint8_t T>
  constexpr void Init(Data &data)
  {
    data.components[T] = new ArrayData<MapToComponentDataType<T>>();
    Temp::Component::Init(*static_cast<Temp::Component::ArrayData<MapToComponentDataType<T>> *>(data.components[T]));
  }

  template <uint8_t T>
  constexpr void Destruct(Data &data)
  {
    delete static_cast<Temp::Component::ArrayData<MapToComponentDataType<T>> *>(data.components[T]);
  }

  template <uint8_t T>
  [[nodiscard]] constexpr ArrayData<MapToComponentDataType<T>> *GetComponentArray(Data &data)
  {
    return static_cast<Temp::Component::ArrayData<MapToComponentDataType<T>> *>(data.components[T]);
  }

  template <uint8_t T>
  [[nodiscard]] constexpr MapToComponentDataType<T> &Get(Data &data, Entity entity)
  {
    // This mess is only needed to make sure we don't need to reference the type twice
    // in the function (once for enum and another for assigned type)
    return Component::Get(*static_cast<Component::ArrayData<MapToComponentDataType<T>> *>(data.components[T]),
                          entity);
  }

  template <uint8_t T>
  constexpr void Set(Data &data, Entity entity, const MapToComponentDataType<T> &component)
  {
    Component::Set(*static_cast<Component::ArrayData<MapToComponentDataType<T>> *>(data.components[T]),
                   entity,
                   component);
  }

  void Init(Data &data);
  void Destruct(Data &data);
  void EntityDestroyed(Data &data, Entity entity);
}
