#include "TextBox.hpp"

#include "Scene.hpp"
#include "FontLoader.hpp"
#include "OpenGLWrapper.hpp"
#include "Drawable.hpp"

// NOTE: Don't use inline global mutexes, it'll stall multiple instances of the same object
namespace Temp::TextBox
{
  namespace
  {
    std::unordered_map<Entity, std::tuple<std::vector<float>, std::vector<unsigned int>>> newData{};
  }

  void ConstructRender(Scene::Data &scene, Data *grid)
  {
    using namespace Temp::Render;

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, grid->entity);
    drawable.texture = Font::Characters['0'].texture;

    Component::Drawable::ConstructFont(drawable, OpenGLWrapper::ShaderIdx::TEXT);

    OpenGLWrapper::Set1IntShaderProperty(drawable.shaderProgram, "text", 0);
    OpenGLWrapper::UnbindBuffers();

    UpdateRender(scene, grid);
  }

  void Construct(Scene::Data &scene, Data *grid)
  {
    grid->entity = Scene::CreateEntity(scene);

    Component::Drawable::Data drawable;
    drawable.entity = grid->entity;
    auto [vertices, indices] = std::move(PopulateVerticesIndices(grid));
    drawable.vertices = std::move(vertices);
    drawable.indices = std::move(indices);

    Scene::AddComponent<Component::Type::DRAWABLE>(scene, grid->entity, std::move(drawable));
    Scene::AddComponent<Component::Type::POSITION2D>(scene, grid->entity, {grid->x, grid->y});
    Scene::AddComponent<Component::Type::SCALE>(scene, grid->entity, grid->scale);
    Scene::AddComponent<Component::Type::TEXT>(scene, grid->entity, std::move(grid->text));
  }
  
  void UpdateText(Scene::Data &scene, Data *grid, const std::string &newText)
  {
    std::lock_guard<std::mutex> lock(*grid->mtx);
    Scene::Get<Temp::Component::Type::TEXT>(scene, grid->entity) = newText;
    grid->text = newText;
    newData[grid->entity] = std::move(PopulateVerticesIndices(grid));
    grid->renderText = true;
  }

  inline void UpdateRender(Scene::Data &scene, Data *grid)
  {
    using namespace Temp::Render;

    std::lock_guard<std::mutex> lock(*grid->mtx);

    if (!grid->renderText)
    {
      return;
    }

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, grid->entity);
//    PopulateVerticesIndices(drawable, grid);
    auto& [vertices, indices] = newData[grid->entity];
    drawable.vertices = std::move(vertices);
    drawable.indices = std::move(indices);

    OpenGLWrapper::UpdateVBO(drawable.VBO, drawable.vertices.data(), drawable.vertices.size(), GL_DYNAMIC_DRAW);
    OpenGLWrapper::UpdateEBO(drawable.EBO, drawable.indices.data(), drawable.indices.size(), GL_DYNAMIC_DRAW);
    
    grid->renderText = false;

    drawable.indicesSize = (int)drawable.indices.size();
  }
  
  std::tuple<std::vector<float>, std::vector<unsigned int>> PopulateVerticesIndices(Data *grid)
  {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    vertices.reserve(grid->text.length() * 16);
    indices.reserve(grid->text.length() * 6);
    
    float x = grid->x;
    float y = grid->y;
    float scale = grid->scale;
    
    // iterate through all characters
    for (unsigned int i = 0; i < grid->text.length(); ++i)
    {
      auto c = grid->text[i];
      Font::Character ch = Font::Characters[c];
      
      float xpos = x + ch.bearing.x * scale;
      float ypos = y - ((float)ch.size.y - ch.bearing.y) * scale;
      
      float w = ch.size.x * scale;
      float h = ch.size.y * scale;
      
      unsigned int offset = 4 * i;
      
      vertices.insert(vertices.end(), {
        // positions      // texture coords
        xpos + w, ypos + h, ch.rectRight, ch.top,    // top right
        xpos + w, ypos, ch.rectRight, ch.rectBottom, // bottom right
        xpos, ypos, ch.left, ch.rectBottom,          // bottom left
        xpos, ypos + h, ch.left, ch.top              // top left
      });
      
      indices.insert(indices.end(), {
        0 + offset, 1 + offset, 3 + offset,
        1 + offset, 2 + offset, 3 + offset
      });
      
      // TODO: Clean this up when no longer needed
      // // positions      // texture coords
      // std::cout << c << std::endl;
      // std::cout << xpos + w << " " << ypos + h << std::endl;
      // std::cout << xpos + w << " " << ypos << std::endl; // bottom right
      // std::cout << xpos << " " << ypos << std::endl;     // bottom left
      // std::cout << xpos << " " << ypos + h << std::endl; // top left
      // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    
    return std::make_tuple(vertices, indices);
  }

  // TODO: Keeping here for reference to NOT do this.
  // Avoid using the Render Queue for real-time updates to avoid flickering
  // inline void UpdateText(Scene::Data *scene, Data *grid, const std::string &newText)
  // {
  //   Scene::Get<Temp::Component::Type::TEXT>(*scene, grid->entity) = newText;
  //   grid->text = newText;
  //   auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(*scene, grid->entity);
  //   PopulateVerticesIndices(drawable, grid);

  //   Scene::EnqueueRender(scene, UpdateRenderVoid, grid);
  // }
}
