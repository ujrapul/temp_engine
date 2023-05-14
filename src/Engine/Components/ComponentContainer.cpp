#include "ComponentContainer.hpp"


namespace Temp
{
  namespace Component
  {
    namespace Container
    {
      namespace
      {
        template<typename T>
        ArrayData<T>* GetComponentArray(Data& data, uint8_t type)
        {
          return static_cast<Temp::Component::ArrayData<T>*>(data.components[type]);
        }
      }

      void Init(Data& data)
      {
        Init<MapToComponentDataType<Type::POSITION2D>>(data, static_cast<uint8_t>(Type::POSITION2D));
      }
      
      void Destruct(Data& data)
      {
        delete GetComponentArray<MapToComponentDataType<Type::POSITION2D>>
          (data, static_cast<uint8_t>(Type::POSITION2D));
      }
      
      void EntityDestroyed(Data& data, Entity entity)
      {
        Component::EntityDestroyed
          (*GetComponentArray<Math::Vec2>(data, static_cast<uint8_t>(Type::POSITION2D)),
           entity);
      }
    }
  }
}
