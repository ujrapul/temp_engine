#pragma once

#include "UT_Common.hpp"
#include "ComponentData.hpp"

namespace Temp::Component::UnitTests
{
  template<typename T>
  inline void TestComponents(ArrayData<T>& data,
                      T Component1,
                      T Component2,
                      T Component3,
                      T Component4,
                      T Component5,
                      T Component6)
  {
    Init(data);
    bool isAllDataInitialized = true;
    for (Entity e = 0; e < MAX_ENTITIES; ++e) {
      isAllDataInitialized &= data.mapping.indexToEntity[e] == UINT_MAX;
      isAllDataInitialized &= data.mapping.entityToIndex[e] == SIZE_MAX;
    }
    Assert("Test ArrayData Init", isAllDataInitialized);
    
    Set(data, 5, Component1);
    Set(data, 15, Component2);
    Set(data, 3, Component3);
    Set(data, 8, Component4);
    Assert("Test ArrayData Set Value Array",
           data.array[0] == Component1
           && data.array[1] == Component2
           && data.array[2] == Component3
           && data.array[3] == Component4
           );
    Assert("Test ArrayData Set Value IndexToEntity",
           data.mapping.indexToEntity[0] == 5
           && data.mapping.indexToEntity[1] == 15
           && data.mapping.indexToEntity[2] == 3
           && data.mapping.indexToEntity[3] == 8
           );
    Assert("Test ArrayData Set Value EntityToIndex",
           data.mapping.entityToIndex[5] == 0
           && data.mapping.entityToIndex[15] == 1
           && data.mapping.entityToIndex[3] == 2
           && data.mapping.entityToIndex[8] == 3
           );
    Assert("Test ArrayData Set Value Size",
           data.mapping.size == 4
           );
    Set(data, 15, Component5);
    Assert("Test ArrayData Set Value to Same Entity Array",
           data.array[1] == Component5
           );
    Assert("Test ArrayData Set Value to Same Entity Size",
           data.mapping.size == 4
           );
    Remove(data, 15);
    Assert("Test ArrayData Remove Value Array",
           data.array[0] == Component1
           && data.array[1] == Component4
           && data.array[2] == Component3
           && data.array[3] == T()
           );
    Assert("Test ArrayData Remove Value Size",
           data.mapping.size == 3
           );
    auto test = Get(data, 3);
    auto test2 = Get(data, 15);
    auto test3 = Get(data, 5);
    auto test4 = T();
    Assert("Test ArrayData Get Value", Get(data, 8) == Component4
           && test2 == test4
           && test == Component3
           && test3 == Component1
           );
    Get(data, 3) = Component6;
    Assert("Test ArrayData Get Value and Modify",
           data.array[data.mapping.entityToIndex[3]] == Component6
           );
    EntityDestroyed(data, 3);
    Assert("Test ArrayData Entity Destroyed Size",
           data.mapping.size == 2
           );
    Assert("Test ArrayData Entity Destroyed Array",
           data.array[0] == Component1
           && data.array[1] == Component4
           && data.array[2] == T()
           && data.array[3] == T()
           );
  }
  
  inline void Run()
  {
    // Simple Component Test
    {
      ArrayData<int> intData{};
      TestComponents(intData, 73234, 2342834, -2382, 999999, 370846540, 2383);
    }
    
    // Complex Component Test
    {
      ArrayData<MockComponent> mockData{};
      TestComponents(mockData, {"q", 234, false}, {"fsa", 478575, false}, {"epjirovagnerj", -10234, true}, {"dblndfb/.324-", 0, true}, {"smcos ", 349654, false}, {"//.,;'[]-=", 3, true});
    }
  }
}
