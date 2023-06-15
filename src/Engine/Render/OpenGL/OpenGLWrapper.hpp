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

  const char **GetShader(int shader);

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

  GLuint CreateVertexShader(int shader);
  GLuint CreateFragmentShader(int shader);
  GLuint CreateShaderProgram(int shader);

  constexpr GLuint CreateVAO()
  {
    GLuint VAO = -1;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
  }

  constexpr GLuint CreateVBO(float *data, size_t arraySize)
  {
    GLuint VBO = -1;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, data, GL_STATIC_DRAW);
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

  constexpr void UpdateVBO(GLuint VBO, void *data, size_t arraySize)
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

  GLuint LoadTexture(const char *texturePath, int imageDataType);

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