#pragma once

#include "Camera.hpp"
#include "Entity.hpp"
#include "FontLoader.hpp"
#include "OpenGLWrapper.hpp"
#include "Scene.hpp"
#include "gl.h"
#include <string>

namespace Temp::TextBox
{
  struct Data
  {
    std::string text;
    float x;
    float y;
    float scale;
    Entity entity;
  };

  inline void ConstructRender(Scene::Data *scene, Data *data)
  {
    using namespace Temp::Render;

    auto &drawable = Scene::Get<Temp::Component::Type::DRAWABLE>(*scene, data->entity);
    drawable.texture = Font::Characters['0'].texture;

    Component::Drawable::Construct(&drawable, OpenGLWrapper::ShaderIdx::TEXT, GL_DYNAMIC_DRAW, 4);

    float orthoScale = 1.f;
    float aspect = 1024.f / 768.f;

    Math::Mat4 projection = Math::Mat4::orthographic(-aspect * orthoScale,
                                                     aspect * orthoScale,
                                                     -orthoScale, orthoScale,
                                                     -100, 100);

    OpenGLWrapper::Set4x4MatrixShaderProperty(drawable.shaderProgram, "projection", &projection.rows[0][0]);
    OpenGLWrapper::Set1IntShaderProperty(drawable.shaderProgram, "text", 0);
    OpenGLWrapper::UnbindBuffers();
  }

  inline void ConstructRenderVoid(Scene::Data *scene, void *data)
  {
    ConstructRender(scene, static_cast<Data *>(data));
  }

  inline void Construct(Scene::Data *scene, Data *data)
  {
    data->entity = Scene::CreateEntity(*scene);

    std::vector<GLuint> indices;
    std::vector<float> vertices;

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

    Component::Drawable::Data drawable;
    drawable.vertices = vertices;
    drawable.indices = indices;

    Scene::AddComponent<Component::Type::DRAWABLE>(*scene, data->entity, drawable);
    Scene::AddComponent<Component::Type::POSITION2D>(*scene, data->entity, {data->x, data->y});
    Scene::AddComponent<Component::Type::SCALE>(*scene, data->entity, data->scale);
    Scene::AddComponent<Component::Type::TEXT>(*scene, data->entity, data->text);

    Scene::EnqueueRender(scene, ConstructRenderVoid, data);
  }
}