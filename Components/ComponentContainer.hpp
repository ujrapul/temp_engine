#include "Position2D.hpp"

namespace Temp
{
  namespace Component
  {
    namespace Container
    {
      enum class ComponentType
      {
        POSITION2D = 0,
      };

      struct Data
      {
        Position2D pos;
      };

      Math::Vec2 getPosition2D(Entity entity, const Data* data)
      {
        return {data->pos.x[entity], data->pos.y[entity]};
      }
    }
  }
}