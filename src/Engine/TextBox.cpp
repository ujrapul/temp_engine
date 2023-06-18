#include "TextBox.hpp"

// NOTE: Don't use inline global mutexes, it'll stall multiple instances of the same object
namespace Temp::TextBox
{
  void ConstructRender(Scene::Data *scene, Data *data)
  {
    using namespace Temp::Render;

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(*scene, data->entity);
    drawable.texture = Font::Characters['0'].texture;

    Component::Drawable::ConstructFont(&drawable, OpenGLWrapper::ShaderIdx::TEXT);

    OpenGLWrapper::Set1IntShaderProperty(drawable.shaderProgram, "text", 0);
    OpenGLWrapper::UnbindBuffers();
  }

  void ConstructRenderVoid(Scene::Data *scene, void *data)
  {
    ConstructRender(scene, static_cast<Data *>(data));
  }

  void Construct(Scene::Data *scene, Data *data)
  {
    data->entity = Scene::CreateEntity(*scene);

    Component::Drawable::Data drawable;
    PopulateVerticesIndices(drawable, data);

    Scene::AddComponent<Component::Type::DRAWABLE>(*scene, data->entity, drawable);
    Scene::AddComponent<Component::Type::POSITION2D>(*scene, data->entity, {data->x, data->y});
    Scene::AddComponent<Component::Type::SCALE>(*scene, data->entity, data->scale);
    Scene::AddComponent<Component::Type::TEXT>(*scene, data->entity, data->text);

    Scene::EnqueueRender(scene, ConstructRenderVoid, data);
  }

  // TODO: Keeping here for reference to NOT do this.
  // Avoid using the Render Queue for real-time updates to avoid flickering
  // inline void UpdateText(Scene::Data *scene, Data *data, const std::string &newText)
  // {
  //   Scene::Get<Temp::Component::Type::TEXT>(*scene, data->entity) = newText;
  //   data->text = newText;
  //   auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(*scene, data->entity);
  //   PopulateVerticesIndices(drawable, data);

  //   Scene::EnqueueRender(scene, UpdateRenderVoid, data);
  // }
}