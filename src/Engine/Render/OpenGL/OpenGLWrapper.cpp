#include "OpenGLWrapper.hpp"

// IMPORTANT NOTES SINCE YOU'RE TOO DUMB TO REMEMBER THEM!
//
// Use glVertexAttribPointer for vec2, GLfloat, GLdouble, and GLubyte
// Use glVertexAttrib"I"Pointer for ivec2, GLint, GLuint, and GLbyte
//

namespace Temp::Render::OpenGLWrapper
{
  const char **GetShader(int shader)
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

  GLuint CreateVertexShader(int shader)
  {
    return CreateShader(GetShader(shader * 2), GL_VERTEX_SHADER);
  }

  GLuint CreateFragmentShader(int shader)
  {
    // We add a '1' since shaders are compiled in two sets.
    return CreateShader(GetShader(shader * 2 + 1), GL_FRAGMENT_SHADER);
  }

  GLuint CreateShaderProgram(int shader)
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

  GLuint LoadTexture(const char *texturePath, int imageDataType)
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
}