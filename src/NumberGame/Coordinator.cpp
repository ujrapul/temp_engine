#include "Coordinator.hpp"
#include "Components/ComponentContainer.hpp"

namespace Game
{
  namespace Coordinator
  {
    void Init(Temp::Coordinator::Data& data)
    {
      Temp::Coordinator::Init(data);
      Component::Container::Init(data.componentData);
    }
    
    void Destruct(Temp::Coordinator::Data& data)
    {
      Temp::Coordinator::Destruct(data);
      Component::Container::Destruct(data.componentData);
    }
  }
}

