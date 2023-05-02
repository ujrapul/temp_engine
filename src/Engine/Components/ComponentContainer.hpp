#pragma once

#include "ComponentData.hpp"
#include "ComponentType.hpp"

namespace Temp
{
  namespace Component
  {
    namespace Container
    {
      struct Data
      {
        Component::ArrayData<Math::Vec2> positions;
      };

      Math::Vec2 getPosition2D(Entity entity,
                               Component::ArrayData<Math::Vec2>& positions);
      void setPosition2D(Component::ArrayData<Math::Vec2>& positions,
                         Entity entity,
                         Math::Vec2 newPos);
      void Init(Data& data);
      void EntityDestroyed(Data& data, Entity entity);
      
      template<typename T>
      T& Get(Data& data, Entity entity, Type type)
      {
        switch (type)
        {
          case Type::POSITION2D:
            return Component::Get(data.positions, entity);
          default:
            break;
        }
      }
    }
  }
}
