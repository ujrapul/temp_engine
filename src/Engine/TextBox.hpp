#pragma once

#include "Entity.hpp"
#include "FontLoader.hpp"
#include "OpenGLWrapper.hpp"
#include "Scene.hpp"
#include "gl.h"
#include <string>

// NOTE: Don't use inline global mutexes, it'll stall multiple instances of the same object
namespace Temp::TextBox
{
  struct Data
  {
    std::string text{};
    float x{};
    float y{};
    float scale{};
    Entity entity{};
  };

  constexpr void UpdateRender(Scene::Data *scene, Data *data)
  {
    using namespace Temp::Render;

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(*scene, data->entity);

    OpenGLWrapper::UpdateVBO(drawable.VBO, drawable.vertices.data(), drawable.vertices.size(), GL_DYNAMIC_DRAW);
    OpenGLWrapper::UpdateEBO(drawable.EBO, drawable.indices.data(), drawable.indices.size(), GL_DYNAMIC_DRAW);
  }

  void ConstructRender(Scene::Data *scene, Data *data);
  void ConstructRenderVoid(Scene::Data *scene, void *data);

  constexpr void PopulateVerticesIndices(Component::Drawable::Data &drawable, Data *data)
  {
    drawable.vertices.clear();
    drawable.indices.clear();
    drawable.vertices.reserve(data->text.length() * 16);
    drawable.indices.reserve(data->text.length() * 6);

    float x = data->x;
    float y = data->y;
    float scale = data->scale;

    // iterate through all characters
    for (unsigned int i = 0; i < data->text.length(); ++i)
    {
      auto c = data->text[i];
      Font::Character ch = Font::Characters[c];

      float xpos = x + ch.bearing.x * scale;
      float ypos = y - ((float)ch.size.y - ch.bearing.y) * scale;

      float w = ch.size.x * scale;
      float h = ch.size.y * scale;

      unsigned int offset = 4 * i;

      drawable.vertices.insert(drawable.vertices.end(), {
                                                            // positions      // texture coords
                                                            xpos + w, ypos + h, ch.rectRight, ch.top,    // top right
                                                            xpos + w, ypos, ch.rectRight, ch.rectBottom, // bottom right
                                                            xpos, ypos, ch.left, ch.rectBottom,          // bottom left
                                                            xpos, ypos + h, ch.left, ch.top              // top left
                                                        });

      drawable.indices.insert(drawable.indices.end(), {0 + offset, 1 + offset, 3 + offset,
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

  void Construct(Scene::Data *scene, Data *data);

  constexpr void UpdateText(Scene::Data *scene, Data *data, const std::string &newText)
  {
    Scene::Get<Temp::Component::Type::TEXT>(*scene, data->entity) = newText;
    data->text = newText;
    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(*scene, data->entity);
    PopulateVerticesIndices(drawable, data);
  }

  constexpr void UpdateTextRender(Scene::Data *scene, Data *data)
  {
    UpdateRender(scene, data);
  }
}