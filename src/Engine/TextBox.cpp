#include "TextBox.hpp"

#include "Scene.hpp"

// NOTE: Don't use inline global mutexes, it'll stall multiple instances of the same object
namespace Temp::TextBox
{
  void ConstructRender(Scene::Data &scene, Data *grid)
  {
    using namespace Temp::Render;

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, grid->entity);
    drawable.texture = Font::Characters['0'].texture;

    Component::Drawable::ConstructFont(&drawable, OpenGLWrapper::ShaderIdx::TEXT);

    OpenGLWrapper::Set1IntShaderProperty(drawable.shaderProgram, "text", 0);
    OpenGLWrapper::UnbindBuffers();

    UpdateRender(scene, grid);
  }

  void Construct(Scene::Data &scene, Data *grid)
  {
    grid->entity = Scene::CreateEntity(scene);

    Component::Drawable::Data drawable;
    drawable.entity = grid->entity;
    PopulateVerticesIndices(drawable, grid);

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
    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, grid->entity);
    PopulateVerticesIndices(drawable, grid);
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

    OpenGLWrapper::UpdateVBO(drawable.VBO, drawable.vertices.data(), drawable.vertices.size(), GL_DYNAMIC_DRAW);
    OpenGLWrapper::UpdateEBO(drawable.EBO, drawable.indices.data(), drawable.indices.size(), GL_DYNAMIC_DRAW);
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