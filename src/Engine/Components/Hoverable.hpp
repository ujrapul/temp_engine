#pragma once

namespace Temp::Scene
{
  struct Data;
}

// TODO: Currently making an assumption that game objects can't be hoverable.
// May change in the future.
namespace Temp::Component::Hoverable
{
  constexpr void NoOp(Scene::Data &, struct Data *) {}

  struct Data
  {
    void (*Click)(Scene::Data &, Data *){NoOp};
    void (*Hover)(Scene::Data &, Data *){NoOp};
    void *callbackData{nullptr};
    float x{};
    float y{};
    float width{};
    float height{};
  };

  // TOD0: Might need to revisit this for other widgets
  //       For now this is assuming the origin is at the lower left-hand corner (this is the case for TextBoxes)
  inline bool IsInside(Data *hoverable, float x, float y)
  {
    auto viewSpaceCoords = Temp::Camera::ConvertToFontOrthoViewSpace(x, y);
    return viewSpaceCoords.x >= hoverable->x && viewSpaceCoords.x <= hoverable->x + hoverable->width &&
           viewSpaceCoords.y >= hoverable->y && viewSpaceCoords.y <= hoverable->y + hoverable->height;
  }
};