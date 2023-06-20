#pragma once

#include "Engine/Components/ComponentContainer.hpp"

namespace Game::Component::Container
{
  void Init(Temp::Component::Container::Data& data);
  void Destruct(Temp::Component::Container::Data& data);
}
