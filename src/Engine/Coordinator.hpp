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
    
    template<typename T>
    void AddComponent(Data& data, Entity entity, T component, Component::Type type)
    {
      Signature sig = EntityManager::GetSignature(data.entityData.signatures, entity);
      sig.set(static_cast<size_t>(type));
      EntityManager::SetSignature(data.entityData.signatures,
                                  entity,
                                  sig);
      Component::Container::Set(data.componentData,
                                entity,
                                component,
                                static_cast<uint8_t>(Component::Type::POSITION2D));
    }
    
    template<typename T>
    void UpdateComponent(Data& data, Entity entity, T component, Component::Type type)
    {
      if (EntityManager::GetSignature(data.entityData.signatures, entity).test(static_cast<size_t>(type)))
        Component::Container::Get<T>(data, entity) = component;
    }
  }
}
