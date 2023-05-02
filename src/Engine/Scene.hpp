#pragma once

#include "Coordinator.hpp"
#include <iostream>

namespace Temp
{
  namespace Scene
  {
    enum class State : uint8_t
    {
      ENTER = 0,
      RUN   = 1,
      LEAVE = 2,
    };
    
    struct Data
    {
      Coordinator::Data coordinator;
      std::array<Entity, MAX_ENTITIES> entities;
      State state{State::ENTER};
      Data* nextScene{nullptr};
      void (*Construct)(Scene::Data*){nullptr};
      void (*Update)(Scene::Data*, float){nullptr};
      void (*Destruct)(Scene::Data*){nullptr};
    };
  }
}
