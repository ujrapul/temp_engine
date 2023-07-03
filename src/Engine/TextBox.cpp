#include "TextBox.hpp"

#include "Scene.hpp"
#include "FontLoader.hpp"
#include "OpenGLWrapper.hpp"
#include "Drawable.hpp"
#include <vector>

// NOTE: Don't use inline global mutexes, it'll stall multiple instances of the same object
namespace Temp::TextBox
{
  namespace
  {
    std::unordered_map<Entity, std::tuple<std::vector<float>, std::vector<unsigned int>>> newData{};

    std::tuple<std::vector<float>, std::vector<unsigned int>> PopulateVerticesIndices(Data &textBox)
    {
      std::vector<float> vertices;
      std::vector<unsigned int> indices;

      vertices.reserve(textBox.text.length() * 16);
      indices.reserve(textBox.text.length() * 6);

      float x = textBox.x;
      float y = textBox.y;
      float scale = textBox.scale;

      // iterate through all characters
      for (unsigned int i = 0; i < textBox.text.length(); ++i)
      {
        auto c = textBox.text[i];
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

        indices.insert(indices.end(), {0 + offset, 1 + offset, 3 + offset,
                                       1 + offset, 2 + offset, 3 + offset});

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
  }

  void DrawConstruct(Scene::Data &scene, Data &textBox)
  {
    using namespace Temp::Render;

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, textBox.entity);
    drawable.texture = Font::Characters['0'].texture;

    Component::Drawable::ConstructFont(drawable, OpenGLWrapper::ShaderIdx::TEXT);

    OpenGLWrapper::Set1IntShaderProperty(drawable.shaderProgram, "text", 0);
    OpenGLWrapper::UnbindBuffers();

    UpdateRender(scene, textBox);
  }

  void Construct(Scene::Data &scene, Data &textBox)
  {
    textBox.entity = Scene::CreateEntity(scene);

    Component::Drawable::Data drawable;
    drawable.entity = textBox.entity;
    auto [vertices, indices] = std::move(PopulateVerticesIndices(textBox));
    drawable.vertices = std::move(vertices);
    drawable.indices = std::move(indices);

    Scene::AddComponent<Component::Type::DRAWABLE>(scene, textBox.entity, std::move(drawable));
    Scene::AddComponent<Component::Type::POSITION2D>(scene, textBox.entity, {textBox.x, textBox.y});
    Scene::AddComponent<Component::Type::SCALE>(scene, textBox.entity, textBox.scale);
    Scene::AddComponent<Component::Type::TEXT>(scene, textBox.entity, std::move(textBox.text));
  }

  void UpdateText(Scene::Data &scene, Data &textBox, const std::string &newText)
  {
    std::lock_guard<std::mutex> lock(*textBox.mtx);
    Scene::Get<Temp::Component::Type::TEXT>(scene, textBox.entity) = newText;
    textBox.text = newText;
    newData[textBox.entity] = std::move(PopulateVerticesIndices(textBox));
    textBox.renderText = true;
  }

  void UpdateRender(Scene::Data &scene, Data &textBox)
  {
    using namespace Temp::Render;

    std::lock_guard<std::mutex> lock(*textBox.mtx);

    if (!textBox.renderText)
    {
      return;
    }

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, textBox.entity);
    //    PopulateVerticesIndices(drawable, textBox);
    auto &[vertices, indices] = newData[textBox.entity];
    drawable.vertices = std::move(vertices);
    drawable.indices = std::move(indices);
    drawable.disableDepth = true;

    OpenGLWrapper::UpdateVBO(drawable.VBO, drawable.vertices.data(), drawable.vertices.size(), GL_DYNAMIC_DRAW);
    OpenGLWrapper::UpdateEBO(drawable.EBO, drawable.indices.data(), drawable.indices.size(), GL_DYNAMIC_DRAW);

    textBox.renderText = false;

    drawable.indicesSize = (int)drawable.indices.size();
  }

  struct EnableOutlineData
  {
    Data *textBox;
    bool enable{false};
  };

  void RenderFunction(Scene::Data &scene, void *renderData)
  {
    auto *enableOutlineData = static_cast<EnableOutlineData *>(renderData);
    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, enableOutlineData->textBox->entity);
    Render::OpenGLWrapper::Set1BoolShaderProperty(drawable.shaderProgram, "u_useOutline", enableOutlineData->enable);
    Render::OpenGLWrapper::Set1FloatShaderProperty(drawable.shaderProgram, "u_thickness", enableOutlineData->enable ? 0.75 : 0.5);
    // Render::OpenGLWrapper::Set1FloatShaderProperty(drawable.shaderProgram, "u_outline_thickness", enableOutlineData->enable ? 0.9 : 0.5);
    delete enableOutlineData;
  }

  void EnableOutline(Scene::Data &scene, Data &textBox, bool enable)
  {
    Scene::EnqueueRender(scene, RenderFunction, new EnableOutlineData{&textBox, enable});
  }
  
  void DrawDestruct(Scene::Data &scene, Data &textBox)
  {
    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, textBox.entity);
    Component::Drawable::Destruct(drawable);
  }

  // TODO: Keeping here for reference to NOT do this.
  // Avoid using the Render Queue for real-time updates to avoid flickering
  // inline void UpdateText(Scene::Data *scene, Data *textBox, const std::string &newText)
  // {
  //   Scene::Get<Temp::Component::Type::TEXT>(*scene, textBox.entity) = newText;
  //   textBox.text = newText;
  //   auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(*scene, textBox.entity);
  //   PopulateVerticesIndices(drawable, textBox);

  //   Scene::EnqueueRender(scene, UpdateRenderVoid, textBox);
  // }
}
