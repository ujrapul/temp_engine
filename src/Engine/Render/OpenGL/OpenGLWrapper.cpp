#include "OpenGLWrapper.hpp"
#include "FontLoader.hpp"

// IMPORTANT NOTES SINCE YOU'RE TOO DUMB TO REMEMBER THEM!
//
// Use glVertexAttribPointer for vec2, GLfloat, GLdouble, and GLubyte
// Use glVertexAttrib"I"Pointer for ivec2, GLint, GLuint, and GLbyte
//

namespace Temp::Render::OpenGLWrapper
{
  namespace
  {
    const std::filesystem::path& GetShadersPath()
    {
      static auto shadersPath = ApplicationDirectory() / "Shaders";
      return shadersPath;
    }

    const char *GetCommonShader()
    {
      static const char* common = LoadFileAsString(std::filesystem::path(GetShadersPath() / "Common.glsl").c_str());
      return common;
    }
  }

  void LoadShaders()
  {
    const auto& shadersPath = GetShadersPath();

    static const char *VERT_HEADER = "#version 330\n#define VERTEX_SHADER\n";
    static const char *FRAG_HEADER = "#version 330\n#define FRAGMENT_SHADER\n";

    static std::vector<const char *> shaderFiles = {"Test.glsl", "Text.glsl", "Grid.glsl"};

    for (const char* shaderFile : shaderFiles)
    {
      globalShaders.insert(globalShaders.end(), {
        {VERT_HEADER, GetCommonShader(), LoadFileAsString(std::filesystem::path(shadersPath / shaderFile).c_str())},
        {FRAG_HEADER, GetCommonShader(), LoadFileAsString(std::filesystem::path(shadersPath / shaderFile).c_str())}});
    }
  }

  GLuint LoadTexture(const char *texturePath, int imageDataType)
  {
    // load image, create texture and generate mipmaps
    int width = 0, height = 0, nrChannels = 0;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if (!data)
    {
      std::cout << "Failed to load texture" << std::endl;
    }
    // load and create a texture
    // -------------------------
    GLuint texture = CreateTexture(imageDataType, width, height, data);

    stbi_image_free(data);

    return texture;
  }

  // TODO: Keeping this for reference! Remove when TextBox can handle vertex buffer changes.
  // void RenderText(GLuint VAO, GLuint VBO, GLuint EBO, GLuint shaderProgram, const std::string &text, float x, float y, float scale)
  // {
  //   // activate corresponding render state
  //   glUseProgram(shaderProgram);
  //   glActiveTexture(GL_TEXTURE0);
  //   glBindVertexArray(VAO);

  //   OpenGLWrapper::Set1IntShaderProperty(shaderProgram, "text", 0);

  //   // render glyph texture over quad
  //   glBindTexture(GL_TEXTURE_2D, Font::Characters['0'].texture);

  //   // iterate through all characters
  //   for (auto c : text)
  //   {
  //     Font::Character ch = Font::Characters[c];

  //     float xpos = x + ch.bearing.x * scale;
  //     float ypos = y - (ch.size.y - ch.bearing.y) * scale;

  //     float w = ch.size.x * scale;
  //     float h = ch.size.y * scale;

  //     float vertices[16] = {
  //         // positions      // texture coords
  //         xpos + w, ypos + h, ch.rectRight, ch.top,    // top right
  //         xpos + w, ypos, ch.rectRight, ch.rectBottom, // bottom right
  //         xpos, ypos, ch.left, ch.rectBottom,          // bottom left
  //         xpos, ypos + h, ch.left, ch.top              // top left
  //     };

  //     // update content of VBO memory
  //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  //     glBindBuffer(GL_ARRAY_BUFFER, 0);
  //     // render quad
  //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  //     // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
  //     x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  //   }
  //   glBindVertexArray(0);
  //   glBindTexture(GL_TEXTURE_2D, 0);
  // }
}