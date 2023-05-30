#include "ComponentContainer.hpp"
#include "ComponentType.hpp"

namespace Game::Component::Container
{
  void Init(Temp::Component::Container::Data& data)
  {
    Temp::Component::Container::Init<Type::VALUE>(data);
    Temp::Component::Container::Init<Type::COLLECTED_VALUE>(data);
    Temp::Component::Container::Init<Type::SCORE>(data);
  }
  
  void Destruct(Temp::Component::Container::Data& data)
  {
    delete Temp::Component::Container::GetComponentArray<Type::VALUE>(data);
    delete Temp::Component::Container::GetComponentArray<Type::COLLECTED_VALUE>(data);
    delete Temp::Component::Container::GetComponentArray<Type::SCORE>(data);
  }
}
