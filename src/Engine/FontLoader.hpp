#pragma once

#include "Math.hpp"
#include "gl.h"
#include "OpenGLWrapper.hpp"
#include <ft2build.h>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <array>
#include <unistd.h>
#include FT_FREETYPE_H

namespace Temp::Font
{
  struct Character
  {
    Math::Vec2ui Size{};   // Size of glyph
    Math::Vec2i Bearing{}; // Offset from baseline to left/top of glyph
    long int Advance{};    // Offset to advance to next glyph
    GLuint TextureID{};    // ID handle of the glyph texture
    float left{};
    float right{};
    float top{};
    float bottom{};
  };

  // TODO: Turn into an accessor function
  inline std::unordered_map<char, Character> Characters;

  inline bool LoadFont()
  {
    FT_Library ft = nullptr;
    if (FT_Init_FreeType(&ft))
    {
      std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
      return false;
    }

    std::filesystem::path fontsPath = ApplicationDirectory / "Fonts" / "Arial.ttf";

    FT_Face face = nullptr;
    if (FT_New_Face(ft, fontsPath.c_str(), 0, &face))
    {
      std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
      return false;
    }

    constexpr int fontSize = 128;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      return false;
    }

    Render::OpenGLWrapper::SetUnpackAlignment(1);

    constexpr uint32_t asciiNum = 128;
    constexpr uint32_t atlasTileWidth = floor(sqrt(asciiNum));
    constexpr uint32_t atlasTileHeight = ceil(sqrt(asciiNum));
    constexpr uint32_t atlasWidth = fontSize * atlasTileWidth;
    constexpr uint32_t atlasHeight = fontSize * atlasTileHeight;
    constexpr uint32_t atlasSize = atlasWidth * atlasHeight;
    uint32_t xOffset = 0;
    uint32_t yOffset = 0;

    std::array<unsigned char, atlasSize> pixels{};

    // generate texture atlas
    GLuint texture = -1;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, (void*)pixels.data());

    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    for (unsigned char c = 0; c < asciiNum; c++)
    {
      // load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      auto textureWidth = face->glyph->bitmap.width;
      auto textureHeight = face->glyph->bitmap.rows;

      glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        xOffset,
        yOffset,
        textureWidth,
        textureHeight,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
      );
      // now store character for later use
      Character character = {
        {textureWidth, textureHeight},                            // size
        {face->glyph->bitmap_left, face->glyph->bitmap_top},      // bearing
        face->glyph->advance.x,                                   // advance
        texture,                                                  // texture id
        static_cast<float>(xOffset) / atlasWidth,                 // left
        static_cast<float>(xOffset + fontSize) / atlasWidth,      // right
        static_cast<float>(yOffset) / atlasHeight,                // top
        static_cast<float>(yOffset + fontSize) / atlasHeight      // bottom
      };
      Characters[c] = character;
      xOffset = (xOffset + fontSize) % atlasWidth;
      if (xOffset == 0)
      {
        yOffset = (yOffset + fontSize) % atlasWidth;
      }
    }

    Render::OpenGLWrapper::SetUnpackAlignment();

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
  }
}