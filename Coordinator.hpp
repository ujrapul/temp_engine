#pragma once

#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"

namespace Temp
{
  namespace Coordinator
  {
    struct Data
    {
      EntityManager::Data entityData;
      Component::Container::Data componentData;
    };
    
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
    
    template<typename T>
    void AddComponent(Data& data, Entity entity, T component, Component::Type type)
    {
      Signature sig = EntityManager::GetSignature(data.entityData.signatures, entity);
      sig.set(static_cast<size_t>(type));
      EntityManager::SetSignature(data.entityData.signatures,
                                  entity,
                                  sig);
      Component::Container::setPosition2D(data.componentData.positions,
                                          entity,
                                          component);
    }
    
    template<typename T>
    void UpdateComponent(Data& data, Entity entity, T component, Component::Type type)
    {
      if (EntityManager::GetSignature(data.entityData.signatures, entity).test(static_cast<size_t>(type)))
        Component::Container::Get<T>(data, entity) = component;
    }
  }
}
