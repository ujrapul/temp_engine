#pragma once

#include "Entity.hpp"
#include "Components/Component.hpp"
#include <queue>
#include <array>

namespace Temp
{
  namespace EntityManager
  {
    using EntityQueue = std::queue<Entity>;
    using SignatureArray = std::array<Signature, MAX_ENTITIES>;

    struct Data
    {
      EntityQueue availableEntities{};
      SignatureArray signatures{};
      uint32_t livingEntityCount{};
    };
    
    void InitData(EntityQueue& availableEntities);
    Entity CreateEntity(EntityQueue& availableEntities, uint32_t& livingEntityCount);
    void DestroyEntity(
      EntityQueue& availableEntities,
      uint32_t& livingEntityCount,
      SignatureArray& signatures,
      Entity entity);
    void SetSignature(SignatureArray& signatures, Entity entity, Signature signature);
    Signature GetSignature(SignatureArray& signatures, Entity entity);
  }
}
