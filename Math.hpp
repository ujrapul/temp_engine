#pragma once

namespace Temp
{
  namespace Math
  {
    struct Vec2
    {
      float x{};
      float y{};
      
      void operator=(const Vec2& other)
      {
        x = other.x;
        y = other.y;
      }
    };

    struct Vec3
    {
      float x{};
      float y{};
      float z{};
    };

    struct Vec4
    {
      float x{};
      float y{};
      float z{};
      float w{};
    };

    using Quat = Vec4;
  }
}
