#include "ComponentContainer.hpp"

namespace Temp
{
  namespace Component
  {
    namespace Container
    {
      void Init(Data& data)
      {
        Init<Type::POSITION2D>(data);
        Init<Type::DRAWABLE>(data);
      }
      
      void Destruct(Data& data)
      {
        Destruct<Type::POSITION2D>(data);
        Destruct<Type::DRAWABLE>(data);
      }
      
      void EntityDestroyed(Data& data, Entity entity)
      {
        Component::EntityDestroyed(*GetComponentArray<Type::POSITION2D>(data), entity);
        Component::EntityDestroyed(*GetComponentArray<Type::DRAWABLE>(data), entity);
      }
    }
  }
}
