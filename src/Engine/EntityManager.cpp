#include "EntityManager.hpp"
#include "Entity.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

namespace Temp
{
  namespace EntityManager
  {
    void InitData(Data &entityManager)
    {
      entityManager = {};
      for (Entity e = 0; e < MAX_ENTITIES; ++e)
      {
        entityManager.availableEntities.push(e);
      }
    }

    Entity CreateEntity(Data &entityManager)
    {
      assert(entityManager.livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

      Entity id = entityManager.availableEntities.front();
      entityManager.availableEntities.pop();
      ++entityManager.livingEntityCount;

      entityManager.currentEntities.push_back(id);

      return id;
    }

    void DestroyEntity(Data &entityManager, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Invalidate the destroyed entity's signature
      entityManager.signatures[entity].reset();

      // Put the destroyed ID at the back of the queue
      entityManager.availableEntities.push(entity);
      --entityManager.livingEntityCount;

      entityManager.currentEntities.erase(std::find(entityManager.currentEntities.begin(), entityManager.currentEntities.end(), entity));

      assert(entityManager.livingEntityCount < MAX_ENTITIES && "Living Entity Count underflowed!");
    }

    void SetSignature(Data& entityManager, Entity entity, Signature signature)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Put this entity's signature into the array
      entityManager.signatures[entity] = signature;
    }

    Signature GetSignature(Data& entityManager, Entity entity)
    {
      assert(entity < MAX_ENTITIES && "Entity out of range.");

      // Get this entity's signature from the array
      return entityManager.signatures[entity];
    }

    void Destruct(Data &entityManager)
    {
      while (!entityManager.currentEntities.empty())
      {
        DestroyEntity(entityManager, entityManager.currentEntities.back());
      }
    }
    
    void Reset(Data& entityManager)
    {
      Destruct(entityManager);
      InitData(entityManager);
    }
  }
}
