#pragma once

#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"

namespace Temp::Coordinator
{
  struct Data
  {
    EntityManager::Data entityData;
    Component::Container::Data componentData;
  };

  [[nodiscard]] Entity CreateEntity(Data &coordinator);
  void DestroyEntity(Data &coordinator, Entity entity);
  void Init(Data &coordinator);
  void Destruct(Data &coordinator);
  [[nodiscard]] const Math::Vec2f &GetPosition(const Data &coordinator, Entity entity);
  void Reset(Data &coordinator);

  template <uint8_t T>
  constexpr void AddComponent(Data &coordinator, Entity entity, const Component::MapToComponentDataType<T>& component)
  {
    Signature sig = EntityManager::GetSignature(coordinator.entityData, entity);
    sig.set(T);
    EntityManager::SetSignature(coordinator.entityData,
                                entity,
                                sig);
    Component::Container::Set<T>(coordinator.componentData,
                                 entity,
                                 component);
  }

  template <typename T>
  constexpr void UpdateComponent(Data &coordinator, Entity entity, T component, uint8_t type)
  {
    if (EntityManager::GetSignature(coordinator.entityData, entity).test(type))
    {
      Component::Container::Get<T>(coordinator, entity) = component;
    }
  }
}
