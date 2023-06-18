#include "EntityManager.hpp"
#include "Entity.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

namespace Temp
{
  namespace EntityManager
  {
    void InitData(Data &data)
    {
      data = {};
      for (Entity e = 0; e < MAX_ENTITIES; ++e)
      {
        data.availableEntities.push(e);
      }
    }

    Entity CreateEntity(Data &data)
    {
      assert(data.livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

      Entity id = data.availableEntities.front();
      data.availableEntities.pop();
      ++data.livingEntityCount;

      data.currentEntities.push_back(id);

      return id;
    }

    void DestroyEntity(Data &data, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Invalidate the destroyed entity's signature
      data.signatures[entity].reset();

      // Put the destroyed ID at the back of the queue
      data.availableEntities.push(entity);
      --data.livingEntityCount;

      data.currentEntities.erase(std::find(data.currentEntities.begin(), data.currentEntities.end(), entity));

      assert(data.livingEntityCount < MAX_ENTITIES && "Living Entity Count underflowed!");
    }

    void SetSignature(SignatureArray &signatures, Entity entity, Signature signature)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Put this entity's signature into the array
      signatures[entity] = signature;
    }

    Signature GetSignature(SignatureArray &signatures, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Get this entity's signature from the array
      return signatures[entity];
    }

    void Destruct(Data &data)
    {
      while (!data.currentEntities.empty())
      {
        DestroyEntity(data, data.currentEntities.back());
      }
    }
  }
}
