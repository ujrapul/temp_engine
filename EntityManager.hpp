#pragma once

#include "Entity.hpp"
#include "Components/Component.hpp"
#include <queue>
#include <array>
#include <cassert>

namespace Temp
{
  namespace EntityManager
  {
    using EntityQueue = std::queue<Entity>;
    using SignatureArray = std::array<Signature, MAX_ENTITIES>;

    struct Data
    {
      EntityQueue availableEntities;
      SignatureArray signatures;
      uint32_t livingEntityCount;
    };

    void CreateEntity(EntityQueue& availableEntities, uint32_t& livingEntityCount)
    {
      assert(livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

      Entity id = availableEntities.front();
      availableEntities.pop();
      ++livingEntityCount;
    }

    void DestroyEntity(
      EntityQueue& availableEntities,
      uint32_t& livingEntityCount,
      SignatureArray& signatures,
      Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Invalidate the destroyed entity's signature
      signatures[entity].reset();

      // Put the destroyed ID at the back of the queue
      availableEntities.push(entity);
      --livingEntityCount;
    }

    void SetSignature(SignatureArray& signatures, Entity entity, Signature signature)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Put this entity's signature into the array
      signatures[entity] = signature;
    }

    Signature GetSignature(SignatureArray& signatures, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Get this entity's signature from the array
      return signatures[entity];
    }
  }
}
