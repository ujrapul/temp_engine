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
    
    void InitData(Data& data);
    [[nodiscard]] Entity CreateEntity(Data& data);
    void DestroyEntity(Data& data, Entity entity);
    void SetSignature(SignatureArray& signatures, Entity entity, Signature signature);
    [[nodiscard]] Signature GetSignature(SignatureArray& signatures, Entity entity);
    void Destruct(Data& data);
  }
}
