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
    
    Entity CreateEntity(Data& data);
    void DestroyEntity(Data& data, Entity entity);
    void Init(Data& data);
    void Destruct(Data& data);
    
    template<uint8_t T, typename Out>
    void AddComponent(Data& data, Entity entity, Out component)
    {
      Signature sig = EntityManager::GetSignature(data.entityData.signatures, entity);
      sig.set(T);
      EntityManager::SetSignature(data.entityData.signatures,
                                  entity,
                                  sig);
      Component::Container::Set<T, Out>(data.componentData,
                                        entity,
                                        component);
    }
    
    template<typename T>
    void UpdateComponent(Data& data, Entity entity, T component, uint8_t type)
    {
      if (EntityManager::GetSignature(data.entityData.signatures, entity).test(type))
        Component::Container::Get<T>(data, entity) = component;
    }
  }
}
