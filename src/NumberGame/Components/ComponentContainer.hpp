#pragma once

#include "Engine/Components/ComponentContainer.hpp"
#include "Entity.hpp"

namespace Game::Component::Container
{
  void Init(Temp::Component::Container::Data& data);
  void Destruct(Temp::Component::Container::Data& data);
  void EntityDestroyed(Temp::Component::Container::Data &data, Temp::Entity entity);
  void Reset(Temp::Component::Container::Data& data);
}
