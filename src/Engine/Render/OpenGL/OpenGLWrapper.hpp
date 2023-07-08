#pragma once

#include "EngineUtils.hpp"
#include "Math.hpp"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include "gl.h"
#endif
#include "stb_image.h"
#include <ft2build.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include FT_FREETYPE_H

// IMPORTANT NOTES SINCE YOU'RE TOO DUMB TO REMEMBER THEM!
//
// Use glVertexAttribPointer for vec2, GLfloat, GLdouble, and GLubyte
// Use glVertexAttrib"I"Pointer for ivec2, GLint, GLuint, and GLbyte
//

namespace Temp::Render::OpenGLWrapper
{
  // DO NOT USE OUTSIDE OPENGLWRAPPER!
  inline std::vector<std::vector<const char *>> globalShaders;

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

  void ClearShaderStrings();
  
  inline const std::vector<const char*>& ShaderFiles()
  {
    static std::vector<const char *> out = {"Test.glsl", "Text.glsl", "Grid.glsl"};
    return out;
  }
  
  inline const std::filesystem::path& GetShadersPath()
  {
    static auto shadersPath = ApplicationDirectory() / "Shaders";
    return shadersPath;
  }
  
  inline const std::vector<std::string>& GlobalShaderFiles()
  {
    static std::vector<std::string> out = {
      (GetShadersPath() / "Common.glsl").c_str()
    };
    return out;
  }
  
  inline std::vector<std::filesystem::file_time_type>& GlobalShaderFilesTimes()
  {
    static std::vector<std::filesystem::file_time_type> out;
    out.resize(GlobalShaderFiles().size());
    return out;
  }

  void LoadShaders();

  inline GLuint CreateShader(const char **shaderSource, int shaderType)
  {
    // Create shader
    GLuint shader = glCreateShader(shaderType);
    constexpr int stringArrayCount = 3;
    glShaderSource(shader, stringArrayCount, shaderSource, nullptr);
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

  constexpr const char **GetShader(int shader)
  {
    return globalShaders[shader].data();
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

  inline GLuint CreateVAO()
  {
    GLuint VAO = -1;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
  }

  inline GLuint CreateVBO(float *data, size_t arraySize, int BufferDraw = GL_STATIC_DRAW)
  {
    GLuint VBO = -1;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, data, BufferDraw);
    return VBO;
  }

  inline GLuint CreateVBO(void *data, size_t typeSize, size_t arraySize, int BufferDraw = GL_STATIC_DRAW)
  {
    GLuint VBO = -1;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, typeSize * arraySize, data, BufferDraw);
    return VBO;
  }

  inline void UpdateVBO(GLuint VBO, void *data, size_t arraySize, int BufferDraw = GL_STATIC_DRAW)
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, data, BufferDraw);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  inline GLuint CreateEBO(GLuint *indices, size_t arraySize, int BufferDraw = GL_STATIC_DRAW)
  {
    // Create Element Buffer Object (EBO) and copy index data
    GLuint EBO = -1;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * arraySize, indices, BufferDraw);
    return EBO;
  }

  inline GLuint UpdateEBO(GLuint EBO, GLuint *indices, size_t arraySize, int BufferDraw = GL_STATIC_DRAW)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * arraySize, indices, BufferDraw);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return EBO;
  }

  inline GLuint CreateUBO(size_t bytes)
  {
    GLuint UBO = -1;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, bytes, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, bytes);
    return UBO;
  }

  inline void BindUBOShader(GLuint UBO, GLuint shaderProgram, const char *property, int index)
  {
    glUniformBlockBinding(shaderProgram, glGetUniformBlockIndex(shaderProgram, property), index);
    glBindBufferBase(GL_UNIFORM_BUFFER, index, UBO);
  }

  inline void UpdateUBO(GLuint UBO, float *data, int size, int offset)
  {
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, (void *)data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  inline void SetVertexAttribArray(int arrayIndex, int numOfElements, int stride, int position)
  {
    glVertexAttribPointer(arrayIndex, numOfElements, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(position * sizeof(float)));
    glEnableVertexAttribArray(arrayIndex);
  }

  inline void SetVertexIAttribArray(int arrayIndex, int numOfElements, int stride, int position)
  {
    glVertexAttribPointer(arrayIndex, numOfElements, GL_INT, GL_FALSE, stride * sizeof(int), (void *)(position * sizeof(int)));
    glEnableVertexAttribArray(arrayIndex);
  }

  // Assumes you're only using float
  inline void SetVertexAttribArrayInstanced(int arrayIndex, int numOfElements, int stride, int position)
  {
    glVertexAttribPointer(arrayIndex, numOfElements, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(position * sizeof(float)));
    glEnableVertexAttribArray(arrayIndex);
    glVertexAttribDivisor(arrayIndex, 1); // Set the attribute to update once per instance
  }

  // Assumes you're only using int
  inline void SetVertexAttribIArrayInstanced(int arrayIndex, int numOfElements, int stride, int position)
  {
    glVertexAttribIPointer(arrayIndex, numOfElements, GL_INT, stride * sizeof(int), (void *)(position * sizeof(int)));
    glEnableVertexAttribArray(arrayIndex);
    glVertexAttribDivisor(arrayIndex, 1); // Set the attribute to update once per instance
  }

  inline void UnbindBuffers()
  {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  GLuint LoadTexture(const char *texturePath, int imageDataType);

  inline GLuint CreateTexture(int imageDataType, int width, int height, void *data, GLint param = GL_REPEAT)
  {
    // load and create a texture
    // -------------------------
    GLuint texture = -1;
    // texture 1
    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, imageDataType, width, height, 0, imageDataType, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
  }

  inline void BindTexture(GLuint textureIndex, GLuint texture)
  {
    glActiveTexture(textureIndex);
    glBindTexture(GL_TEXTURE_2D, texture);
  }

  inline void Set1IntShaderProperty(GLuint shaderProgram, const char *property, int value)
  {
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, property), value);
  }

  inline void Set1FloatShaderProperty(GLuint shaderProgram, const char *property, float value)
  {
    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, property), value);
  }

  inline void Set1BoolShaderProperty(GLuint shaderProgram, const char *property, bool value)
  {
    Set1IntShaderProperty(shaderProgram, property, value);
  }

  inline void Set4x4MatrixShaderProperty(GLuint shaderProgram, const char *property, float *matrix)
  {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, property), 1, GL_TRUE, matrix);
  }

  inline void DrawArrays(GLuint vao, int numVertices)
  {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindVertexArray(0);
  }

  inline void DrawElements(GLuint vao, int numIndices)
  {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  inline void DrawElementsInstanced(GLuint vao, int numIndices, int numInstances)
  {
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0, numInstances);
    glBindVertexArray(0);
  }

  inline void CleanArrays(GLuint &vao)
  {
    glBindVertexArray(vao);
    glDeleteVertexArrays(1, &vao);
  }

  inline void CleanArrayBuffer(GLuint &buffer)
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glDeleteBuffers(1, &buffer);
  }

  inline void CleanElementBuffer(GLuint &buffer)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glDeleteBuffers(1, &buffer);
  }

  inline void CleanShader(GLuint &shader)
  {
    glDeleteProgram(shader);
  }

  // Default is 4, use 1 for Font Textures
  // 1 to disable byte-alignment restriction
  inline void SetUnpackAlignment(int alignment = 4)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
  }

  inline GLuint GenerateFontTexture(FT_Face face)
  {
    return CreateTexture(GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer, GL_CLAMP_TO_EDGE);
  }

  inline void UpdateSubTexture(int xOffset, int yOffset, int textureWidth, int textureHeight, void *data)
  {
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        xOffset,
        yOffset,
        textureWidth,
        textureHeight,
        GL_RED,
        GL_UNSIGNED_BYTE,
        data);
  }
}
