#include "Coordinator.hpp"
#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"

namespace Temp
{
  namespace Coordinator
  {
    Entity CreateEntity(Data &data)
    {
      return EntityManager::CreateEntity(data.entityData);
    }

    void DestroyEntity(Data &data, Entity entity)
    {
      Component::Container::EntityDestroyed(data.componentData, entity);
      EntityManager::DestroyEntity(data.entityData, entity);
    }

    void Init(Data &data)
    {
      EntityManager::InitData(data.entityData);
      Component::Container::Init(data.componentData);
    }

    void Destruct(Data &data)
    {
      Component::Container::Destruct(data.componentData);
      EntityManager::Destruct(data.entityData);
    }

    Math::Vec2f &GetPosition(Data &data, Entity entity)
    {
      return Component::Container::Get<Component::Type::POSITION2D>(data.componentData, entity);
    }
  }
}
