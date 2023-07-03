#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "FontLoader.hpp"
#include <vector>
#include <condition_variable>

namespace Game::Grid
{
  struct Data
  {
    std::vector<int> gridTranslations{};
    std::vector<float> gridUVOffsets{};
    std::vector<char> gridValues{};
    Temp::Entity entity{};
    int gridSize{50};
    GLuint uvVBO;
    GLuint translationVBO;
    GLuint uvOffsetVBO;
    bool updateVBO{false};
    std::mutex* mtx{new std::mutex()};
  };

  void Construct(Temp::Scene::Data &data, Data *grid);
  void DrawConstruct(Temp::Scene::Data &data, Data *grid);
  // Should only be called on the RenderThread
  void DrawUpdate(Temp::Scene::Data &data, Data *grid);
  void UpdateNumbers(Temp::Scene::Data &sceneData, Data *grid, Temp::Entity player, int currentValue);
  void Destruct(Data *grid);
  void DrawDestruct(Temp::Scene::Data &data, Data *grid);
}
