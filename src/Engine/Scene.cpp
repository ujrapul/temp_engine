#include "Scene.hpp"

namespace Temp
{
  namespace Scene
  {
    void Destruct(Data& data)
    {
      Temp::Coordinator::Destruct(data.coordinator);
    }
    
    Entity CreateEntity(Data& data)
    {
      return Coordinator::CreateEntity(data.coordinator);
    }
    
    void DestroyEntity(Data& data, Entity entity)
    {
      Coordinator::DestroyEntity(data.coordinator, entity);
    }
    
    Math::Vec2& GetPosition(Data& data, Entity entity)
    {
      return Coordinator::GetPosition(data.coordinator, entity);
    }
  }
}
