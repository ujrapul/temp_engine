#include "ComponentContainer.hpp"

namespace Temp
{
  namespace Component
  {
    namespace Container
    {
      Math::Vec2 getPosition2D(Entity entity,
                               Component::ArrayData<Math::Vec2>& positions)
      {
        return Component::Get(positions, entity);
      }
      
      void setPosition2D(Component::ArrayData<Math::Vec2>& positions,
                         Entity entity,
                         Math::Vec2 newPos)
      {
        Component::Set(positions, entity, newPos);
      }
      
      void Init(Data& data)
      {
        Component::Init(data.positions);
      }
      
      void EntityDestroyed(Data& data, Entity entity)
      {
        Component::EntityDestroyed(data.positions, entity);
      }
    }
  }
}
