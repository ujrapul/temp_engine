#pragma once

#include "Math.hpp"
#include "gl.h"
#include <unordered_map>

namespace Temp::Font
{
  struct Character
  {
    Math::Vec2ui size{};   // Size of glyph
    Math::Vec2i bearing{}; // Offset from baseline to left/top of glyph
    long int advance{};    // Offset to advance to next glyph
    GLuint texture{};      // ID handle of the glyph texture
    float left{};
    float right{};
    float top{};
    float bottom{};
    float rectRight{};
    float rectBottom{};
    uint32_t atlasWidth{};
    uint32_t atlasHeight{};
  };

  // TODO: Turn into an accessor function
  inline std::unordered_map<char, Character> Characters;

  void LoadFont();
}