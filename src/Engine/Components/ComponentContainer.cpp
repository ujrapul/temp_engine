#include "ComponentContainer.hpp"

namespace Temp::Component::Container
{
  namespace
  {
    constexpr uint8_t ENUM_MIN = 0;
    constexpr uint8_t ENUM_MAX = Type::MAX - 1;
    
    template <uint8_t E>
    void InitEnum(Data &data)
    {
      Init<E>(data);
    }
    
    template <uint8_t E>
    void DestructEnum(Data &data)
    {
      // Destruct logic for enum E
      Destruct<E>(data);
    }
    
    template <uint8_t E>
    void EntityDestroyedEnum(Data &data, Entity entity)
    {
      Component::EntityDestroyed(*GetComponentArray<E>(data), entity);
    }
    
    template <uint8_t E>
    struct EnumRange
    {
      static void InitEnums(Data &data)
      {
        InitEnum<E>(data);
        if constexpr (E < ENUM_MAX)
          EnumRange<E + 1>::InitEnums(data);
      }
      
      static void DestructEnums(Data &data)
      {
        DestructEnum<E>(data); // Call DestructEnum for the current enum value
        if constexpr (E < ENUM_MAX)
          EnumRange<E + 1>::DestructEnums(data); // Recursively call for the next enum value
      }
      
      static void EntityDestroyedEnums(Data &data, Entity entity)
      {
        EntityDestroyedEnum<E>(data, entity);
        if constexpr (E < ENUM_MAX)
          EnumRange<E + 1>::EntityDestroyedEnums(data, entity);
      }
    };
  }
  
  void Init(Data &data)
  {
    EnumRange<ENUM_MIN>::InitEnums(data);
  }
  
  void Destruct(Data &data)
  {
    EnumRange<ENUM_MIN>::DestructEnums(data);
  }
  
  void EntityDestroyed(Data &data, Entity entity)
  {
    EnumRange<ENUM_MIN>::EntityDestroyedEnums(data, entity);
  }
}
