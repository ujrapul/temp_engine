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
      }
      
      void Destruct(Data& data)
      {
        delete GetComponentArray<Type::POSITION2D>(data);
      }
      
      void EntityDestroyed(Data& data, Entity entity)
      {
        Component::EntityDestroyed(*GetComponentArray<Type::POSITION2D>(data), entity);
      }
    }
  }
}
