#pragma once

#include "Entity.hpp"
#include "FontLoader.hpp"
#include "OpenGLWrapper.hpp"
#include "Drawable.hpp"
#include "gl.h"
#include <string>

namespace Temp::Scene
{
  struct Data;
}

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
    bool renderText{false};
    std::mutex *mtx{new std::mutex()};
  };

  void ConstructRender(Scene::Data &scene, Data *grid);
  void Construct(Scene::Data &scene, Data *grid);
  void UpdateText(Scene::Data &scene, Data *grid, const std::string &newText);
  void UpdateRender(Scene::Data &scene, Data *grid);

  constexpr void PopulateVerticesIndices(Component::Drawable::Data &drawable, Data *grid)
  {
    drawable.vertices.clear();
    drawable.indices.clear();
    drawable.vertices.reserve(grid->text.length() * 16);
    drawable.indices.reserve(grid->text.length() * 6);

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
}