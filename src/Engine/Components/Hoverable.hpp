#pragma once

#include "Camera.hpp"

namespace Temp::Scene
{
  struct Data;
}

// TODO: Currently making an assumption that game objects can't be hoverable.
//       May change in the future.
namespace Temp::Component::Hoverable
{
  constexpr void NoOp(Scene::Data &, struct Data &) {}
  constexpr void NoOpDrag(Scene::Data &, struct Data &, float, float) {}

  struct Data
  {
    void (*Click)(Scene::Data &, Data &){NoOp};
    void (*HoverEnter)(Scene::Data &, Data &){NoOp};
    void (*HoverLeave)(Scene::Data &, Data &){NoOp};
    void (*Drag)(Scene::Data &, Data &, float, float){NoOpDrag};
    void *callbackData{nullptr};
    float x{0};
    float y{0};
    float width{0};
    float height{0};
    bool lastInside{false};
    bool isDrag{false};
    
    constexpr bool operator==(const Data& other) const = default;
  };

  // TOD0: Might need to revisit this for other widgets
  //       For now this is assuming the origin is at the lower left-hand corner (this is the case for TextBoxes)
  inline bool IsInside(const Data &hoverable, float x, float y)
  {
    auto viewSpaceCoords = Camera::ConvertToFontOrthoViewSpace(x, y);
    // std::cout << viewSpaceCoords.x << " " << viewSpaceCoords.y << " " << hoverable.x << " " << hoverable.y << std::endl;
    return viewSpaceCoords.x >= hoverable.x && viewSpaceCoords.x <= hoverable.x + hoverable.width &&
           viewSpaceCoords.y >= hoverable.y && viewSpaceCoords.y <= hoverable.y + hoverable.height;
  }

  inline void HoverableEnter(Scene::Data &scene, Data &hoverable)
  {
    if (!hoverable.lastInside)
    {
      hoverable.HoverEnter(scene, hoverable);
      hoverable.lastInside = true;
    }
  }

  inline void HoverableLeave(Scene::Data &scene, Data &hoverable)
  {
    if (hoverable.lastInside)
    {
      hoverable.HoverLeave(scene, hoverable);
      hoverable.lastInside = false;
    }
  }

  inline void Drag(Scene::Data& scene, Data &hoverable, float lastX, float lastY, float currX, float currY)
  {
    if (!hoverable.isDrag)
    {
      return;
    }

    auto currViewSpaceCoords = Camera::ConvertToFontOrthoViewSpace(currX, currY);
    auto lastViewSpaceCoords = Camera::ConvertToFontOrthoViewSpace(lastX, lastY);
    hoverable.x += (currViewSpaceCoords.x - lastViewSpaceCoords.x);
    hoverable.y += (currViewSpaceCoords.y - lastViewSpaceCoords.y);
    hoverable.Drag(scene, hoverable, hoverable.x, hoverable.y);
  }
};
