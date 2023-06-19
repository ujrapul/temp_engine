#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "FontLoader.hpp"
#include <vector>

namespace Game::Grid
{
  struct Data
  {
    std::vector<Temp::Entity> gridEntities{};
    std::vector<int> gridTranslations{};
    std::vector<float> gridUVOffsets{};
    Temp::Entity entity{};
    int gridSize{50};
    GLuint uvVBO;
    GLuint translationVBO;
    GLuint uvOffsetVBO;
  };

  void Construct(Temp::Scene::Data *data, Data *grid);
  void ConstructRender(Temp::Scene::Data *data, Data *grid);
  // Should only be called on the RenderThread
  void UpdateUVOffsets(Temp::Scene::Data *data, Data *grid);
  void UpdateNumbers(Temp::Scene::Data *sceneData, Data *grid, Temp::Entity player, int currentValue);
}
