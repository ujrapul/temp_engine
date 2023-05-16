#include "Coordinator.hpp"
#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"

namespace Temp
{
  namespace Coordinator
  {
    Entity CreateEntity(Data& data)
    {
      return EntityManager::CreateEntity(data.entityData.availableEntities,
                                         data.entityData.livingEntityCount);
    }
    
    void DestroyEntity(Data& data, Entity entity)
    {
      Component::Container::EntityDestroyed(data.componentData, entity);
      EntityManager::DestroyEntity(data.entityData.availableEntities,
                                   data.entityData.livingEntityCount,
                                   data.entityData.signatures,
                                   entity);
    }
    
    void Init(Data& data)
    {
      EntityManager::InitData(data.entityData.availableEntities);
      Component::Container::Init(data.componentData);
    }
    
    void Destruct(Data& data)
    {
      Component::Container::Destruct(data.componentData);
    }
    
    Math::Vec2& GetPosition(Data& data, Entity entity)
    {
      return Component::Container::Get<Component::Type::POSITION2D>(data.componentData, entity);
    }
  }
}
