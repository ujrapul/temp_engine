#include "Components/ComponentContainer.hpp"
#include "bits/stdc++.h"

namespace Temp
{
  namespace Engine
  {
    void Run()
    {
      Temp::Component::Container::Data *componentData =
          (Temp::Component::Container::Data *)malloc(sizeof(Temp::Component::Container::Data));
      std::cout << "Position: "
        << Temp::Component::Container::getPosition2D(MAX_ENTITIES/2, componentData).x
        << " "
        << Temp::Component::Container::getPosition2D(MAX_ENTITIES/2, componentData).y
        << std::endl;
      free(componentData);
    }
  }
}