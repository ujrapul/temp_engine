#include "FontLoader.hpp"

#include "OpenGLWrapper.hpp"
#include "Math.hpp"
#include <iostream>
#include <filesystem>
#include <array>
#include <unistd.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Temp::Font
{
  void LoadFont()
  {
    FT_Library ft = nullptr;
    if (FT_Init_FreeType(&ft))
    {
      std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
      return;
    }
    
    std::filesystem::path fontsPath = ApplicationDirectory() / "Fonts" / "Arial.ttf";
    
    FT_Face face = nullptr;
    if (FT_New_Face(ft, fontsPath.c_str(), 0, &face))
    {
      std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
      return;
    }
    
    constexpr int fontSize = 128;
    
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      return;
    }
    
    Render::OpenGLWrapper::SetUnpackAlignment(1);
    
    constexpr uint32_t padding = 16;
    constexpr uint32_t asciiNum = 128;
    constexpr uint32_t atlasTileWidth = Math::Floor(Math::Sqrt(asciiNum));
    constexpr uint32_t atlasTileHeight = Math::Ceil(Math::Sqrt(asciiNum));
    constexpr uint32_t atlasWidth = (fontSize + padding * 2) * atlasTileWidth;
    constexpr uint32_t atlasHeight = (fontSize + padding * 2) * atlasTileHeight;
    constexpr uint32_t atlasSize = atlasWidth * atlasHeight;
    uint32_t xOffset = 0;
    uint32_t yOffset = 0;
    
    // We used a gigantic array before which came out to be around 2 mil+ items...
    // Don't do that...
    std::vector<unsigned char> pixels;
    pixels.resize(atlasSize);
    
    GLuint texture = Render::OpenGLWrapper::CreateTexture(GL_RED, atlasWidth, atlasHeight, pixels.data(), GL_CLAMP_TO_EDGE);
    
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
      
      Render::OpenGLWrapper::UpdateSubTexture(xOffset + padding, yOffset + padding, textureWidth, textureHeight, face->glyph->bitmap.buffer);
      
      // now store character for later use
      Character character = {
        {textureWidth + padding * 2, textureHeight + padding * 2},                             // size
        {face->glyph->bitmap_left, face->glyph->bitmap_top},       // bearing
        face->glyph->advance.x + ((padding) << 6),                                    // advance
        texture,                                                   // texture id
        static_cast<float>(xOffset) / atlasWidth,                  // left
        static_cast<float>(xOffset + fontSize) / atlasWidth,       // right
        static_cast<float>(yOffset) / atlasHeight,                 // top
        static_cast<float>(yOffset + fontSize) / atlasHeight,      // bottom
        static_cast<float>(xOffset + textureWidth + padding * 2) / atlasWidth,   // rectRight
        static_cast<float>(yOffset + textureHeight + padding * 2) / atlasHeight, // rectBottom
        atlasWidth,
        atlasHeight};
      Characters[c] = character;
      xOffset = (xOffset + fontSize + padding * 2) % atlasWidth;
      if (xOffset == 0)
      {
        yOffset = (yOffset + fontSize + padding * 2) % atlasHeight;
      }
    }
    
    Render::OpenGLWrapper::SetUnpackAlignment();
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
  }
}
