#include "Scene.hpp"

namespace Temp
{
  namespace Scene
  {
    void Destruct(Data& data)
    {
      Temp::Coordinator::Destruct(data.coordinator);
    }
  }
}
