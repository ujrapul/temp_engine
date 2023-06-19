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
    bool isUpdateVBOFinished{true};
    bool destructing{false};
    std::mutex* mtx{new std::mutex()};
    std::condition_variable cv;
  };

  void Construct(Temp::Scene::Data *data, Data *grid);
  void ConstructRender(Temp::Scene::Data *data, Data *grid);
  // Should only be called on the RenderThread
  void UpdateUVOffsets(Temp::Scene::Data *data, Data *grid);
  void UpdateNumbers(Temp::Scene::Data *sceneData, Data *grid, Temp::Entity player, int currentValue);
  void Destruct(Data *grid);
}
