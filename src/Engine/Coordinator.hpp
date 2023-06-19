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

  [[nodiscard]] Entity CreateEntity(Data &data);
  void DestroyEntity(Data &data, Entity entity);
  void Init(Data &data);
  void Destruct(Data &data);
  [[nodiscard]] Math::Vec2f &GetPosition(Data &data, Entity entity);

  template <uint8_t T>
  constexpr void AddComponent(Data &data, Entity entity, Component::MapToComponentDataType<T> component)
  {
    Signature sig = EntityManager::GetSignature(data.entityData.signatures, entity);
    sig.set(T);
    EntityManager::SetSignature(data.entityData.signatures,
                                entity,
                                sig);
    Component::Container::Set<T>(data.componentData,
                                 entity,
                                 component);
  }

  template <typename T>
  constexpr void UpdateComponent(Data &data, Entity entity, T component, uint8_t type)
  {
    if (EntityManager::GetSignature(data.entityData.signatures, entity).test(type))
      Component::Container::Get<T>(data, entity) = component;
  }
}
