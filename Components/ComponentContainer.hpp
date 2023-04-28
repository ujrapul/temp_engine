#include "Position2D.hpp"

namespace Temp
{
  namespace Component
  {
    namespace Container
    {
      struct Data
      {
        std::array<Math::Vec2, MAX_ENTITIES> pos;
      };

      Math::Vec2 getPosition2D(Entity entity, const Data& data)
      {
        return data.pos[entity];
      }
      
      void setPosition2D(Entity entity,
                         std::array<Math::Vec2, MAX_ENTITIES>& positions,
                         Math::Vec2 newPos)
      {
        positions[entity] = newPos;
      }
    }
  }
}
