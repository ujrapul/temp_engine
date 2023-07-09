#include "TextBox.hpp"

#include "Scene.hpp"
#include "SceneObject.hpp"
#include "FontLoader.hpp"
#include "OpenGLWrapper.hpp"
#include "Drawable.hpp"
#include "EngineUtils.hpp"
#include <vector>

// NOTE: Don't use inline global mutexes, it'll stall multiple instances of the same object
namespace Temp::TextBox
{
  namespace
  {
    void PopulateVerticesIndices(Data &textBox)
    {
      auto &vertices = textBox.vertices;
      auto &indices = textBox.indices;

      FreeContainer(vertices);
      FreeContainer(indices);

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
    }
  }

  void DrawConstruct(Scene::Data &scene, Data &textBox)
  {
    using namespace Temp::Render;

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, textBox.entity);
    drawable.texture = Font::Characters['0'].texture;
    Component::Drawable::UpdateData(drawable, std::move(textBox.vertices), std::move(textBox.indices));

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
    PopulateVerticesIndices(textBox);
    textBox.mtx = new std::mutex();
    textBox.renderText = false;
    textBox.enableOutline = false;

    Scene::AddComponent<Component::Type::DRAWABLE>(scene, textBox.entity, std::move(drawable));
    Scene::AddComponent<Component::Type::POSITION2D>(scene, textBox.entity, {textBox.x, textBox.y});
    Scene::AddComponent<Component::Type::SCALE>(scene, textBox.entity, textBox.scale);
    Scene::AddComponent<Component::Type::TEXT>(scene, textBox.entity, textBox.text);
  }

  void UpdateText(Scene::Data &scene, Data &textBox, const std::string &newText)
  {
    std::lock_guard<std::mutex> lock(*textBox.mtx);
    Scene::Get<Temp::Component::Type::TEXT>(scene, textBox.entity) = newText;
    textBox.text = newText;
    PopulateVerticesIndices(textBox);
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
    Component::Drawable::UpdateData(drawable, std::move(textBox.vertices), std::move(textBox.indices));
    drawable.disableDepth = true;

    // NOTE: This is referring to updating the drawable buffers manually using the OpenGLWrapper
    // Don't know why this won't update properly if I pass textBox vertices and indices directly
    // Removing std::move from DrawConstruct doesn't work...
    Temp::Component::Drawable::UpdateVertexIndexBuffers(drawable, GL_DYNAMIC_DRAW);
    FreeContainer(textBox.vertices);
    FreeContainer(textBox.indices);

    textBox.renderText = false;
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

#ifdef DEBUG
  void DrawReload(Scene::Data &scene, Data &textBox, int shaderIdx)
#else
  void DrawReload(Scene::Data &, Data &, int)
#endif
  {
#ifdef DEBUG
    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(scene, textBox.entity);
    if (shaderIdx != drawable.shaderIdx)
    {
      return;
    }

    PopulateVerticesIndices(textBox);
    DrawDestruct(scene, textBox);
    DrawConstruct(scene, textBox);
#endif
  }

  void Destruct(Data &textBox)
  {
    FreeContainer(textBox.vertices);
    FreeContainer(textBox.indices);
    delete textBox.mtx;
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
