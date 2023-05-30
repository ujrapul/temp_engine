#pragma once

#include "Engine/Components/ComponentContainer.hpp"

namespace Game::Component::Container
{
  void Init(Temp::Component::Container::Data& data);
  void Destruct(Temp::Component::Container::Data& data);
  
  template<uint8_t T>
  Temp::Component::MapToComponentDataType<T>& Get(Temp::Component::Container::Data& data, Temp::Entity entity)
  {
    return Temp::Component::Container::Get<T>(data, entity);
  }
}
