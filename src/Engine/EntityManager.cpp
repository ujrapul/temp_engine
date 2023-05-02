#include "EntityManager.hpp"
#include "Entity.hpp"
#include <cassert>

namespace Temp
{
  namespace EntityManager
  {
    void InitData(EntityQueue& availableEntities)
    {
      for (Entity e = 0; e < MAX_ENTITIES; ++e) {
        availableEntities.push(e);
      }
    }

    Entity CreateEntity(EntityQueue& availableEntities, uint32_t& livingEntityCount)
    {
      assert(livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

      Entity id = availableEntities.front();
      availableEntities.pop();
      ++livingEntityCount;
      
      return id;
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
