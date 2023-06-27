#include "Coordinator.hpp"
#include "Components/ComponentContainer.hpp"
#include "Components/ComponentType.hpp"
#include "EntityManager.hpp"

namespace Temp
{
  namespace Coordinator
  {
    Entity CreateEntity(Data &coordinator)
    {
      return EntityManager::CreateEntity(coordinator.entityData);
    }

    void DestroyEntity(Data &coordinator, Entity entity)
    {
      Component::Container::EntityDestroyed(coordinator.componentData, entity);
      EntityManager::DestroyEntity(coordinator.entityData, entity);
    }

    void Init(Data &coordinator)
    {
      EntityManager::InitData(coordinator.entityData);
      Component::Container::Init(coordinator.componentData);
    }

    void Destruct(Data &coordinator)
    {
      Component::Container::Destruct(coordinator.componentData);
      EntityManager::Destruct(coordinator.entityData);
    }

    Math::Vec2f &GetPosition(Data &coordinator, Entity entity)
    {
      return Component::Container::Get<Component::Type::POSITION2D>(coordinator.componentData, entity);
    }
  }
}
