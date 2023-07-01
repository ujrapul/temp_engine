#pragma once

#include "UT_Common.hpp"
#include "ComponentContainer.hpp"

namespace Temp::Component::Container::UnitTests
{
  template<uint8_t T>
  void RunGeneralAsserts(Data& data)
  {
    Assert("Test Component Container Init Equality Test",
           static_cast<Temp::Component::ArrayData<MapToComponentDataType<T>> *>(data.components[T])->array[0]
           == MapToComponentDataType<T>()
           );
    Assert("Test Component Container Get Array",
           GetComponentArray<T>(data).array[0] == MapToComponentDataType<T>()
           && GetComponentArray<T>(data).array[MAX_ENTITIES - 1] == MapToComponentDataType<T>()
           );
    Assert("Test Component Container Get Component from Entity",
           Get<T>(data, 15) == MapToComponentDataType<T>()
           && Get<T>(data, MAX_ENTITIES - 1) == MapToComponentDataType<T>()
           );
    Set<T>(data, 15, GetTestValue<T>());
    Assert("Test Component Container Set Component of Entity",
           Get<T>(data, 15) == GetTestValue<T>()
           && Get<T>(data, 15) != MapToComponentDataType<T>()
           );
  }
  
  template<uint8_t T>
  void Run()
  {
    Data data{};
    
    Init<T>(data);
    bool isAllDataInitialized = true;
    auto *arrayData = static_cast<Temp::Component::ArrayData<MapToComponentDataType<T>> *>(data.components[T]);
    for (Entity e = 0; e < MAX_ENTITIES; ++e) {
      isAllDataInitialized &= arrayData->mapping.indexToEntity[e] == UINT_MAX;
      isAllDataInitialized &= arrayData->mapping.entityToIndex[e] == SIZE_MAX;
    }
    Assert("Test Component Container Init", isAllDataInitialized);
    RunGeneralAsserts<T>(data);
    Destruct<T>(data);
    Assert("Test Component Container Destruct",
           data.components[T] == nullptr
           );
  }
  
  constexpr uint8_t ENUM_MIN = 0;
  constexpr uint8_t ENUM_MAX = Type::HOVERABLE; // Don't want to test new types
  
  template <uint8_t E>
  void RunEnum()
  {
    Run<E>();
  }
  
  template <uint8_t E>
  void InitializeEnum(const Data& data, bool& isInitialized)
  {
    const auto * const arrayData = static_cast<Temp::Component::ArrayData<MapToComponentDataType<E>> *>(data.components[E]);
    for (Entity e = 0; e < MAX_ENTITIES; ++e) {
      isInitialized &= arrayData->mapping.indexToEntity[e] == UINT_MAX;
      isInitialized &= arrayData->mapping.entityToIndex[e] == SIZE_MAX;
    }
  }
  
  template <uint8_t E>
  void IsDestructEnum(const Data& data, bool& isDestruct)
  {
    isDestruct &= data.components[E] == nullptr;
  }
  
  template <uint8_t E>
  struct EnumRange
  {
    static void RunEnums()
    {
      RunEnum<E>();
      if constexpr (E < ENUM_MAX)
        EnumRange<E + 1>::RunEnums();
    }
    
    static void InitializeEnums(const Data& data, bool& isInitialized)
    {
      InitializeEnum<E>(data, isInitialized);
      if constexpr (E < ENUM_MAX)
        EnumRange<E + 1>::InitializeEnums(data, isInitialized);
    }
    
    static void IsDestructEnums(const Data& data, bool isDestruct)
    {
      IsDestructEnum<E>(data, isDestruct);
      if constexpr (E < ENUM_MAX)
        EnumRange<E + 1>::IsDestructEnums(data, isDestruct);
    }
  };
  
  void Run()
  {
    EnumRange<ENUM_MIN>::RunEnums();
    
    Data data{};
    Init(data);
    bool isAllDataInitialized = true;
    EnumRange<ENUM_MIN>::InitializeEnums(data, isAllDataInitialized);
    Assert("Test Component Container Init All", isAllDataInitialized);
    
    Set<0>(data, 15, GetTestValue<0>());
    Set<3>(data, 15, GetTestValue<3>());
    EntityDestroyed(data, 15);
    Assert("Test Component Container Entity Destroyed",
           Get<0>(data, 15) == MapToComponentDataType<0>()
           && Get<3>(data, 15) == MapToComponentDataType<3>()
           );
    
    Destruct(data);
    bool isAllDataDestructed = true;
    EnumRange<ENUM_MIN>::IsDestructEnums(data, isAllDataDestructed);
    Assert("Test Component Container Destruct All", isAllDataDestructed);
  }
}
