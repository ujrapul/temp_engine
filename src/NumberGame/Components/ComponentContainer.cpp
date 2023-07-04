#include "ComponentContainer.hpp"
#include "ComponentType.hpp"

namespace Game::Component::Container
{
  namespace
  {
    constexpr uint8_t ENUM_MIN = Type::VALUE;
    constexpr uint8_t ENUM_MAX = Type::MAX - 1;

    template <uint8_t E>
    void InitEnum(Temp::Component::Container::Data &data)
    {
      Init<E>(data);
    }

    template <uint8_t E>
    void DestructEnum(Temp::Component::Container::Data &data)
    {
      // Destruct logic for enum E
      Destruct<E>(data);
    }

    template <uint8_t E>
    void EntityDestroyedEnum(Temp::Component::Container::Data &data, Temp::Entity entity)
    {
      Temp::Component::EntityDestroyed(GetComponentArray<E>(data), entity);
    }

    template <uint8_t E>
    void ResetEnum(Temp::Component::Container::Data &data)
    {
      // Destruct logic for enum E
      Reset<E>(data);
    }

    template <uint8_t E>
    struct EnumRange
    {
      static void InitEnums(Temp::Component::Container::Data &data)
      {
        InitEnum<E>(data);
        if constexpr (E < ENUM_MAX)
          EnumRange<E + 1>::InitEnums(data);
      }

      static void DestructEnums(Temp::Component::Container::Data &data)
      {
        DestructEnum<E>(data); // Call DestructEnum for the current enum value
        if constexpr (E < ENUM_MAX)
          EnumRange<E + 1>::DestructEnums(data); // Recursively call for the next enum value
      }

      static void EntityDestroyedEnums(Temp::Component::Container::Data &data, Temp::Entity entity)
      {
        EntityDestroyedEnum<E>(data, entity);
        if constexpr (E < ENUM_MAX)
          EnumRange<E + 1>::EntityDestroyedEnums(data, entity);
      }
      
      static void ResetEnums(Temp::Component::Container::Data &data)
      {
        ResetEnum<E>(data);
        if constexpr (E < ENUM_MAX)
          EnumRange<E + 1>::ResetEnums(data);
      }
    };
  }

  void Init(Temp::Component::Container::Data &data)
  {
    EnumRange<ENUM_MIN>::InitEnums(data);
  }

  void Destruct(Temp::Component::Container::Data &data)
  {
    EnumRange<ENUM_MIN>::DestructEnums(data);
  }

  void EntityDestroyed(Temp::Component::Container::Data &data, Temp::Entity entity)
  {
    EnumRange<ENUM_MIN>::EntityDestroyedEnums(data, entity);
  }
  
  void Reset(Temp::Component::Container::Data &data)
  {
    EnumRange<ENUM_MIN>::ResetEnums(data);
  }
}
