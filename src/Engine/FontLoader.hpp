#pragma once

#include "Math.hpp"
#include <ft2build.h>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <unistd.h>
#include FT_FREETYPE_H

namespace Temp::Font
{
  struct Character
  {
    GLuint TextureID{};    // ID handle of the glyph texture
    Math::Vec2ui Size{};   // Size of glyph
    Math::Vec2i Bearing{}; // Offset from baseline to left/top of glyph
    long int Advance{};    // Offset to advance to next glyph
  };

  // TODO: Turn into an accessor function
  static std::unordered_map<char, Character> Characters;

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

    FT_Set_Pixel_Sizes(face, 0, 512);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      return false;
    }

    Render::OpenGLWrapper::SetUnpackAlignment(1);

    for (unsigned char c = 0; c < 128; c++)
    {
      // load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      // generate texture
      unsigned int texture = Render::OpenGLWrapper::GenerateFontTexture(face);
      // now store character for later use
      Character character = {
          texture,
          {face->glyph->bitmap.width, face->glyph->bitmap.rows},
          {face->glyph->bitmap_left, face->glyph->bitmap_top},
          face->glyph->advance.x};
      Characters[c] = character;
    }

    Render::OpenGLWrapper::SetUnpackAlignment();

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
  }
}