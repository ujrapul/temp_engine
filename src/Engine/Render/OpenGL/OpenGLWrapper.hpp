#pragma once

#include "stb_image.h"
#include "gl.h"
#include "EngineUtils.hpp"
#include <iostream>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

// IMPORTANT NOTES SINCE YOU'RE TOO DUMB TO REMEMBER THEM!
//
// Use glVertexAttribPointer for vec2, GLfloat, GLdouble, and GLubyte
// Use glVertexAttrib"I"Pointer for ivec2, GLint, GLuint, and GLbyte
//

namespace Temp::Render::OpenGLWrapper
{
  namespace ShaderIdx
  {
    enum ShaderIdx
    {
      TEST,
      TEXT,
      GRID,
      MAX
    };
  };

  inline const char **GetShader(int shader)
  {
    std::filesystem::path shadersPath = ApplicationDirectory / "Shaders";

    static const char *VERT_HEADER = "#version 330\n#define VERTEX_SHADER\n";
    static const char *FRAG_HEADER = "#version 330\n#define FRAGMENT_SHADER\n";
    static std::vector<std::vector<const char *>> globalShaders = {
        {VERT_HEADER, LoadFileAsString(std::filesystem::path(shadersPath / "Test.glsl").c_str())},
        {FRAG_HEADER, LoadFileAsString(std::filesystem::path(shadersPath / "Test.glsl").c_str())},

        {VERT_HEADER, LoadFileAsString(std::filesystem::path(shadersPath / "Text.glsl").c_str())},
        {FRAG_HEADER, LoadFileAsString(std::filesystem::path(shadersPath / "Text.glsl").c_str())},

        {VERT_HEADER, LoadFileAsString(std::filesystem::path(shadersPath / "Grid.glsl").c_str())},
        {FRAG_HEADER, LoadFileAsString(std::filesystem::path(shadersPath / "Grid.glsl").c_str())}};

    return globalShaders[shader].data();
  }

  constexpr GLuint CreateShader(const char **shaderSource, int shaderType)
  {
    // Create shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 2, shaderSource, nullptr);
    glCompileShader(shader);

    // Check for shader compilation errors
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      char infoLog[512] = {};
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      switch (shaderType)
      {
      case GL_VERTEX_SHADER:
        std::cerr << "Vertex shader compilation failed\n"
                  << infoLog << std::endl;
        break;
      case GL_FRAGMENT_SHADER:
        std::cerr << "Fragment shader compilation failed\n"
                  << infoLog << std::endl;
        break;
      default:
        break;
      }
      return -1;
    }
    return shader;
  }

  inline GLuint CreateVertexShader(int shader)
  {
    return CreateShader(GetShader(shader * 2), GL_VERTEX_SHADER);
  }

  inline GLuint CreateFragmentShader(int shader)
  {
    // We add a '1' since shaders are compiled in two sets.
    return CreateShader(GetShader(shader * 2 + 1), GL_FRAGMENT_SHADER);
  }

  inline GLuint CreateShaderProgram(int shader)
  {
    GLuint vertexShader = CreateVertexShader(shader);
    GLuint fragmentShader = CreateFragmentShader(shader);

    // Create shader program and link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success = 0;

    // Check for shader program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
      char infoLog[512] = {};
      glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
      std::cerr << "Shader program linking failed\n"
                << infoLog << std::endl;
      return -1;
    }

    // Clean up shaders (they are already linked into the program)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
  }

  constexpr GLuint CreateVAO()
  {
    GLuint VAO = -1;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
  }

  constexpr GLuint CreateVBO(float *vertices, size_t arraySize)
  {
    GLuint VBO = -1;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW);
    return VBO;
  }

  constexpr GLuint CreateVBO(void *data, size_t typeSize, size_t arraySize)
  {
    GLuint VBO = -1;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, typeSize * arraySize, data, GL_STATIC_DRAW);
    return VBO;
  }

  constexpr GLuint CreateEBO(GLuint *indices, size_t arraySize)
  {
    // Create Element Buffer Object (EBO) and copy index data
    GLuint EBO = -1;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * arraySize, indices, GL_STATIC_DRAW);
    return EBO;
  }

  constexpr void SetVertexAttribArray(size_t arrayIndex, size_t numOfElements, size_t stride, size_t position)
  {
    glVertexAttribPointer(arrayIndex, numOfElements, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(position * sizeof(float)));
    glEnableVertexAttribArray(arrayIndex);
  }

  // Assumes you're only using int
  constexpr void SetVertexAttribIArrayInstanced(size_t arrayIndex, size_t numOfElements, size_t stride, size_t position)
  {
    glVertexAttribIPointer(arrayIndex, numOfElements, GL_INT, stride * sizeof(int), (void *)(position * sizeof(int)));
    glEnableVertexAttribArray(arrayIndex);
    glVertexAttribDivisor(arrayIndex, 1); // Set the attribute to update once per instance
  }

  // Assumes you're only using float
  constexpr void SetVertexAttribArrayInstanced(size_t arrayIndex, size_t numOfElements, size_t stride, size_t position)
  {
    glVertexAttribPointer(arrayIndex, numOfElements, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(position * sizeof(float)));
    glEnableVertexAttribArray(arrayIndex);
    glVertexAttribDivisor(arrayIndex, 1); // Set the attribute to update once per instance
  }

  constexpr void UnbindBuffers()
  {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  inline GLuint LoadTexture(const char *texturePath, int imageDataType)
  {
    // load and create a texture
    // -------------------------
    GLuint texture = -1;
    // texture 1
    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width = 0, height = 0, nrChannels = 0;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, imageDataType, width, height, 0, imageDataType, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
  }

  constexpr void BindTexture(GLuint textureIndex, GLuint texture)
  {
    glActiveTexture(textureIndex);
    glBindTexture(GL_TEXTURE_2D, texture);
  }

  constexpr void Set1IntShaderProperty(GLuint shaderProgram, const char *property, size_t index)
  {
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, property), index);
  }

  constexpr void Set4x4MatrixShaderProperty(GLuint shaderProgram, const char *property, float *matrix)
  {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, property), 1, GL_TRUE, matrix);
  }

  constexpr void DrawArrays(GLuint vao, int numTriangles)
  {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, numTriangles);
    glBindVertexArray(0);
  }

  constexpr void DrawElements(GLuint vao, int numIndices)
  {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  constexpr void DrawElementsInstanced(GLuint vao, int numIndices, int numInstances)
  {
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0, numInstances);
    glBindVertexArray(0);
  }

  constexpr void CleanArrays(GLuint &vao)
  {
    glDeleteVertexArrays(1, &vao);
  }

  constexpr void CleanBuffer(GLuint &buffer)
  {
    glDeleteBuffers(1, &buffer);
  }

  constexpr void CleanShader(GLuint &shader)
  {
    glDeleteProgram(shader);
  }

  // Default is 4, use 1 for Font Textures
  // 1 to disable byte-alignment restriction
  constexpr void SetUnpackAlignment(int alignment = 4)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
  }

  constexpr GLuint GenerateFontTexture(FT_Face face)
  {
    GLuint texture = -1;
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

    return texture;
  }
}