#include "Grid.hpp"
#include "Components/ComponentType.hpp"

namespace Game::Grid
{
  namespace
  {
    std::vector<float> &Vertices()
    {
      using namespace Temp;

      static std::vector<float> vertices = {
          // positions
          0.5f, 0.5f, 0.0f,   // top right
          0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f, // bottom left
          -0.5f, 0.5f, 0.0f,  // top left
      };

      return vertices;
    }

    std::vector<float> &TexCoords()
    {
      using namespace Temp;

      static std::vector<float> texCoords = {
          // texture coords
          Font::Characters[0].right, 0.0f,                       // top right
          Font::Characters[0].right, Font::Characters[0].bottom, // bottom right
          0.0f, Font::Characters[0].bottom,                      // bottom left
          0.0f, 0.0f                                             // top left
      };

      return texCoords;
    }

    std::vector<GLuint> &Indices()
    {
      using namespace Temp;

      // Define the indices
      static std::vector<GLuint> indices = {
          0, 1, 3, // First Triangle
          1, 2, 3  // Second Triangle
      };

      return indices;
    }
  }

  void Construct(Temp::Scene::Data &data, Data *grid)
  {
    using namespace Temp;
    using namespace Temp::Render;

    grid->entity = Temp::Scene::CreateEntity(data);
    Temp::Scene::AddComponent<Temp::Component::Type::DRAWABLE>(data, grid->entity, {});

    grid->gridValues.clear();
    grid->gridTranslations.clear();
    grid->gridUVOffsets.clear();
    grid->mtx = new std::mutex();

    auto &drawable = Temp::Scene::Get<Temp::Component::Type::DRAWABLE>(data, grid->entity);

    drawable.numInstances = grid->gridSize * grid->gridSize;

    srand(static_cast<uint>(time(nullptr)));
    for (auto row = 0; row < grid->gridSize; ++row)
    {
      for (auto col = 0; col < grid->gridSize; ++col)
      {
        grid->gridValues.push_back('0' + rand() % 10);
        grid->gridTranslations.push_back(-grid->gridSize / 2 + row);
        grid->gridTranslations.push_back(-grid->gridSize / 2 + col);
      }
    }
  }

  void DrawConstruct(Temp::Scene::Data &data, Data *grid)
  {
    using namespace Temp;
    using namespace Temp::Render;

    std::lock_guard<std::mutex> lock(*grid->mtx);

    grid->gridUVOffsets.reserve(grid->gridSize * grid->gridSize);
    for (size_t i = 0; i < grid->gridValues.size(); ++i)
    {
      auto value = grid->gridValues[i];
      grid->gridUVOffsets.push_back(Font::Characters[value].left);
      grid->gridUVOffsets.push_back(Font::Characters[value].top);
    }

    auto &drawable = Temp::Scene::Get<Temp::Component::Type::DRAWABLE>(data, grid->entity);
    Temp::Component::Drawable::UpdateData(drawable, Vertices(), Indices());
    Temp::Component::Drawable::Construct(drawable, Temp::Render::OpenGLWrapper::ShaderIdx::GRID);
    grid->uvVBO = Temp::Component::Drawable::CreateFloatBuffer(drawable, TexCoords(), 1, 2, 2, 0);
    grid->translationVBO = Temp::Component::Drawable::CreateIntInstancedBuffer(drawable, grid->gridTranslations, 2, 2, 2, 0);
    grid->uvOffsetVBO = Temp::Component::Drawable::CreateFloatInstancedBuffer(drawable, grid->gridUVOffsets, 3, 2, 2, 0);

    // load and create a texture
    // -------------------------
    // drawable.texture = OpenGLWrapper::LoadTexture((AssetsDirectory() / "Images" / "awesomeface.png").c_str(), GL_RGBA);
    drawable.texture = Font::Characters[0].texture;

    // Needs to be called to set variables in the shader!
    OpenGLWrapper::Set1IntShaderProperty(drawable.shaderProgram, "texture1", 0);

    // Unbind VAO and VBO and EBO
    OpenGLWrapper::UnbindBuffers();
  }

  // Add one layer of indirection so that the function isn't called before construction occurs
  void DrawUpdate(Temp::Scene::Data &/*data*/, Data *grid)
  {
    using namespace Temp;
    using namespace Temp::Render;

    std::unique_lock<std::mutex> lock(*grid->mtx);

    if (!grid->updateVBO)
    {
      return;
    }

    Temp::Component::Drawable::UpdateFloatBuffer(grid->uvOffsetVBO, grid->gridUVOffsets);
    grid->updateVBO = false;
  }

  void UpdateNumbers(Temp::Scene::Data &sceneData, Data *grid, Temp::Entity player, int currentValue)
  {
    using namespace Temp;
    using namespace Temp::Render;

    std::unique_lock<std::mutex> lock(*grid->mtx);

    for (size_t i = 0; i < grid->gridValues.size(); ++i)
    {
      auto &value = grid->gridValues[i];
      if (value - '0' == currentValue)
      {
        value = '-';
        Game::Scene::Player::addScore(player, sceneData, 1);
      }
      grid->gridUVOffsets[i * 2] = Font::Characters[value].left;
      grid->gridUVOffsets[i * 2 + 1] = Font::Characters[value].top;
    }

    grid->updateVBO = true;
  }

  void Destruct(Data *grid)
  {
    delete grid->mtx;
  }
  
  void DrawDestruct(Temp::Scene::Data &data, Data *grid)
  {
    auto &drawable = Temp::Scene::Get<Temp::Component::Type::DRAWABLE>(data, grid->entity);
    Temp::Component::Drawable::Destruct(drawable);
  }
  
  void DrawReload(Temp::Scene::Data &data, Data *grid)
  {
    DrawDestruct(data, grid);
    DrawConstruct(data, grid);
  }
}
