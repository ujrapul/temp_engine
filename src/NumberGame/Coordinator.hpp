#pragma once

#include "Engine/Coordinator.hpp"

namespace Game
{
  namespace Coordinator
  {
    void Init(Temp::Coordinator::Data& data);
    void Destruct(Temp::Coordinator::Data& data);
    void Reset(Temp::Coordinator::Data& data);
  }
}
