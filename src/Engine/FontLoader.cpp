#include "FontLoader.hpp"

#include "OpenGLWrapper.hpp"
#include <iostream>
#include <filesystem>
#include <array>
#include <unistd.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Temp::Font
{
  constexpr double abs(double x)
  {
    if (x < 0)
    {
      return -x;
    }
    return x;
  }
  
  constexpr double sqrt(double x) {
    if (x == 0 || x == 1) {
      return x;
    }
    
    double guess = x / 2;
    double prevGuess;
    
    do {
      prevGuess = guess;
      guess = (guess + x / guess) / 2;
    } while (abs(guess - prevGuess) >= 0.00001);  // Adjust the precision as needed
    
    return guess;
  }
  
  constexpr int ceil(double x) {
    int result = static_cast<int>(x);
    return (result < x) ? (result + 1) : result;
  }
  
  constexpr int floor(double x) {
    int result = static_cast<int>(x);
    return (result > x) ? (result - 1) : result;
  }
  
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
    
    constexpr uint32_t asciiNum = 128;
    constexpr uint32_t atlasTileWidth = floor(sqrt(asciiNum));
    constexpr uint32_t atlasTileHeight = ceil(sqrt(asciiNum));
    constexpr uint32_t atlasWidth = fontSize * atlasTileWidth;
    constexpr uint32_t atlasHeight = fontSize * atlasTileHeight;
    constexpr uint32_t atlasSize = atlasWidth * atlasHeight;
    uint32_t xOffset = 0;
    uint32_t yOffset = 0;
    
    std::array<unsigned char, atlasSize> pixels{};
    
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
      
      Render::OpenGLWrapper::UpdateSubTexture(xOffset, yOffset, textureWidth, textureHeight, face->glyph->bitmap.buffer);
      
      // now store character for later use
      Character character = {
        {textureWidth, textureHeight},                             // size
        {face->glyph->bitmap_left, face->glyph->bitmap_top},       // bearing
        face->glyph->advance.x,                                    // advance
        texture,                                                   // texture id
        static_cast<float>(xOffset) / atlasWidth,                  // left
        static_cast<float>(xOffset + fontSize) / atlasWidth,       // right
        static_cast<float>(yOffset) / atlasHeight,                 // top
        static_cast<float>(yOffset + fontSize) / atlasHeight,      // bottom
        static_cast<float>(xOffset + textureWidth) / atlasWidth,   // rectRight
        static_cast<float>(yOffset + textureHeight) / atlasHeight, // rectBottom
        atlasWidth,
        atlasHeight};
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
  }
}
