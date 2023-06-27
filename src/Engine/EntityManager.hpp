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
      std::vector<Entity> currentEntities{};
      uint32_t livingEntityCount{};
    };
    
    void InitData(Data& entityManager);
    [[nodiscard]] Entity CreateEntity(Data& entityManager);
    void DestroyEntity(Data& entityManager, Entity entity);
    void SetSignature(Data& entityManager, Entity entity, Signature signature);
    [[nodiscard]] Signature GetSignature(Data& entityManager, Entity entity);
    void Destruct(Data& entityManager);
  }
}
