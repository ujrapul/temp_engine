#pragma once

namespace Temp::Scene
{
  struct Data;
}

namespace Temp::Component::Hoverable
{
  constexpr void NoOp(Scene::Data *, struct Data *) {}

  struct Data
  {
    void (*ClickCallback)(Scene::Data *, Data *){NoOp};
    float x{};
    float y{};
    float width{};
    float height{};
  };

  constexpr bool IsInside(Data *hoverable, float x, float y)
  {
    std::cout << x << " " << y << std::endl;
    return x >= hoverable->x && x <= hoverable->x + hoverable->width &&
           y >= hoverable->y && y <= hoverable->y + hoverable->height;
  }

  constexpr void Hover(Data * /*hoverable*/)
  {
  }

  constexpr void Click(Scene::Data *scene, Data *hoverable)
  {
    hoverable->ClickCallback(scene, hoverable);
  }
};