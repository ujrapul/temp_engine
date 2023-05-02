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
  }
}
