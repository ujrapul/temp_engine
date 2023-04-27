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
        std::array<Position2D, MAX_ENTITIES> pos;
      };

      Math::Vec2 getPosition2D(Entity entity, const Data* data)
      {
        return {data->pos[entity].x, data->pos[entity].y};
      }
    }
  }
}
