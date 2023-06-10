#pragma once

#include "stb_image.h"
#include "../glad/gl.h"
#include "stb_image.h"
#include <iostream>

namespace Temp::Render::OpenGLWrapper
{
  // Vertex shader source code
  constexpr const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)";

  // Fragment shader source code
  constexpr const char *fragmentShaderSource = R"(
    #version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture2, TexCoord).r);
  vec4 color = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
  FragColor = mix(texture(texture1, TexCoord), color, 1.0);
}
)";

  constexpr GLuint CreateShader(const char *shaderSource, int shaderType)
  {
    // Create vertex shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for vertex shader compilation errors
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      char infoLog[512] = {};
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      std::cerr << "Vertex shader compilation failed\n"
                << infoLog << std::endl;
      return -1;
    }
    return shader;
  }

  constexpr GLuint CreateVertexShader(const char *shaderSource)
  {
    return CreateShader(shaderSource, GL_VERTEX_SHADER);
  }

  constexpr GLuint CreateFragmentShader(const char *shaderSource)
  {
    return CreateShader(shaderSource, GL_FRAGMENT_SHADER);
  }

  constexpr GLuint CreateShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
  {
    GLuint vertexShader = CreateVertexShader(vertexShaderSource);
    GLuint fragmentShader = CreateFragmentShader(fragmentShaderSource);

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

  constexpr GLuint CreateEBO(GLuint *indices, size_t arraySize)
  {
    // Create Element Buffer Object (EBO) and copy index data
    GLuint EBO = -1;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * arraySize, indices, GL_STATIC_DRAW);
    return EBO;
  }

  constexpr void SetVertexAttribArray(size_t arrayIndex, size_t numOfElements, size_t rowStride, size_t stride)
  {
    glVertexAttribPointer(arrayIndex, numOfElements, GL_FLOAT, GL_FALSE, rowStride * sizeof(float), (void *)(stride * sizeof(float)));
    glEnableVertexAttribArray(arrayIndex);
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

  constexpr void CleanArrays(GLuint &vao)
  {
    glDeleteVertexArrays(1, &vao);
  }

  constexpr void CleanBuffer(GLuint &buffer)
  {
    glDeleteBuffers(1, &buffer);
  }

  constexpr void cleanShader(GLuint& shader)
  {
    glDeleteProgram(shader);
  }
}