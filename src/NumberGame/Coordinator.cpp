#include "Coordinator.hpp"
#include "Components/ComponentContainer.hpp"

namespace Game::Coordinator
{
  void Init(Temp::Coordinator::Data &data)
  {
    Temp::Coordinator::Init(data);
    Component::Container::Init(data.componentData);
  }

  void Destruct(Temp::Coordinator::Data &data)
  {
    Temp::Coordinator::Destruct(data);
    Component::Container::Destruct(data.componentData);
  }

  void Reset(Temp::Coordinator::Data &data)
  {
    Temp::Coordinator::Reset(data);
    Component::Container::Reset(data.componentData);
  }
}
