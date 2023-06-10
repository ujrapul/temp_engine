#pragma once

#include "Math.hpp"
#include "gl.h"
#include <ft2build.h>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <unistd.h>
#include FT_FREETYPE_H

namespace Temp::Font
{
  // Vertex shader source code
  constexpr const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

  // Fragment shader source code
  constexpr const char *fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
)";

  struct Character
  {
    GLuint TextureID{};    // ID handle of the glyph texture
    Math::Vec2ui Size{};   // Size of glyph
    Math::Vec2i Bearing{}; // Offset from baseline to left/top of glyph
    long int Advance{};    // Offset to advance to next glyph
  };

  std::unordered_map<char, Character> Characters;

  inline bool LoadFont()
  {
    FT_Library ft = nullptr;
    if (FT_Init_FreeType(&ft))
    {
      std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
      return false;
    }

    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len == -1)
    {
      std::cout << "FAILED TO READ EXEC DIR!";
      return false;
    }

    std::filesystem::path executablePath(buffer);
    std::filesystem::path fontsPath = executablePath.parent_path() / "Fonts" / "Arial.ttf";

    FT_Face face = nullptr;
    if (FT_New_Face(ft, fontsPath.c_str(), 0, &face))
    {
      std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
      return false;
    }

    FT_Set_Pixel_Sizes(face, 0, 512);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      return -1;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
      // load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      // generate texture
      unsigned int texture = -1;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          face->glyph->bitmap.width,
          face->glyph->bitmap.rows,
          0,
          GL_RED,
          GL_UNSIGNED_BYTE,
          face->glyph->bitmap.buffer);
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      Character character = {
          texture,
          {face->glyph->bitmap.width, face->glyph->bitmap.rows},
          {face->glyph->bitmap_left, face->glyph->bitmap_top},
          face->glyph->advance.x};
      Characters[c] = character;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
  }
}