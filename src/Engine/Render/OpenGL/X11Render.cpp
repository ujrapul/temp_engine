#include "X11Render.hpp"
#include "Input.hpp"
#include "../glad/gl.h"
#include <chrono>
#include <thread>
#include <condition_variable>
#include <iostream>
#include "../glad/glx.h"
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define MIN_KEYCODE 8

namespace Temp::Render
{
  namespace
  {
    // Vertex shader source code
    const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
)";

    // Fragment shader source code
    const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        // Calculate the rainbow color based on the fragment's position
        vec3 rainbowColor;
        rainbowColor.x = (gl_FragCoord.x / 640);  // Vary the color based on the fragment's x position
        rainbowColor.y = (gl_FragCoord.y / 480);  // Vary the color based on the fragment's y position
        rainbowColor.z = 0.5;                       // Set the blue component to a fixed value

        FragColor = vec4(rainbowColor, 1.0);
    }
)";

    float deltaTime{};

    // X11 Display and Window variables
    Display *display{nullptr};
    Window window{};

    // GLX variables
    GLXContext context{};
    XVisualInfo *visualInfo{};
    Colormap colormap{};

    std::thread renderThread{};

    constexpr int window_width = 1024, window_height = 768;

    bool quit = false;

    void RenderThread()
    {
      // Make the OpenGL context current for the rendering thread
      glXMakeCurrent(display, window, context);

      auto start = std::chrono::high_resolution_clock::now();

      // Create vertex shader
      GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
      glCompileShader(vertexShader);

      // Check for vertex shader compilation errors
      GLint success;
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed\n"
                  << infoLog << std::endl;
        return;
      }

      // Create fragment shader
      GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
      glCompileShader(fragmentShader);

      // Check for fragment shader compilation errors
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed\n"
                  << infoLog << std::endl;
        return;
      }

      // Create shader program and link shaders
      GLuint shaderProgram = glCreateProgram();
      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);
      glLinkProgram(shaderProgram);

      // Check for shader program linking errors
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
      if (!success)
      {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed\n"
                  << infoLog << std::endl;
        return;
      }

      // Clean up shaders (they are already linked into the program)
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      // Define the square vertices
      float vertices[] = {
          // Positions
          -0.5f, 0.5f, 0.0f, // Top Left
          0.5f, 0.5f, 0.0f,  // Top Right
          0.5f, -0.5f, 0.0f, // Bottom Right
          -0.5f, -0.5f, 0.0f // Bottom Left
      };

      // Define the indices
      GLuint indices[] = {
          0, 1, 3, // First Triangle
          1, 2, 3  // Second Triangle
      };

      // Create vertex array object (VAO)
      GLuint VAO;
      glGenVertexArrays(1, &VAO);
      glBindVertexArray(VAO);

      // Create vertex buffer object (VBO)
      GLuint VBO;
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      // Create Element Buffer Object (EBO) and copy index data
      GLuint EBO;
      glGenBuffers(1, &EBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      // Specify vertex attributes
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
      glEnableVertexAttribArray(0);

      // Unbind VAO and VBO
      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      // Set the clear color and enable depth testing
      glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
      glEnable(GL_DEPTH_TEST);

      // Main rendering loop
      while (!quit)
      {
        auto stop = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
        start = stop;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Perform your OpenGL rendering here

        // Use the shader program
        glUseProgram(shaderProgram);

        // Bind the VAO and draw the triangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glXSwapBuffers(display, window);

        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
      }

      // Clean up resources
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteProgram(shaderProgram);
    }

    void CreateDisplay(const char *windowName)
    {
      // Open the X11 display
      display = XOpenDisplay(NULL);
      if (display == NULL)
      {
        std::cerr << "Unable to open X11 display" << std::endl;
        return;
      }

      // Get the default screen and root window
      int screen = DefaultScreen(display);

      int glx_version = gladLoaderLoadGLX(display, screen);
      if (!glx_version)
      {
        printf("Unable to load GLX.\n");
        return;
      }
      printf("Loaded GLX %d.%d\n", GLAD_VERSION_MAJOR(glx_version), GLAD_VERSION_MINOR(glx_version));

      Window rootWindow = RootWindow(display, screen);

      // Create an OpenGL-capable visual
      int attribs[] = {
          GLX_RGBA,
          GLX_DOUBLEBUFFER,
          GLX_DEPTH_SIZE, 24,
          GLX_RED_SIZE, 8,
          GLX_GREEN_SIZE, 8,
          GLX_BLUE_SIZE, 8,
          None};

      visualInfo = glXChooseVisual(display, screen, attribs);
      if (visualInfo == NULL)
      {
        std::cerr << "Unable to find a suitable visual" << std::endl;
        return;
      }

      // Create a colormap for the window
      XSetWindowAttributes windowAttribs;
      windowAttribs.colormap = XCreateColormap(display, rootWindow, visualInfo->visual, AllocNone);
      windowAttribs.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
      colormap = windowAttribs.colormap;

      // Create the window
      window = XCreateWindow(display, rootWindow, 0, 0, window_width, window_height, 0,
                             visualInfo->depth, InputOutput, visualInfo->visual,
                             CWColormap | CWEventMask, &windowAttribs);

      // Set the window title
      XStoreName(display, window, windowName);

      // Create the OpenGL context
      context = glXCreateContext(display, visualInfo, NULL, GL_TRUE);
      if (context == NULL)
      {
        std::cerr << "Unable to create an OpenGL context" << std::endl;
        return;
      }

      glXMakeCurrent(display, window, context);

      // Map and show the window
      XMapWindow(display, window);
      XFlush(display);

      int gl_version = gladLoaderLoadGL();
      if (!gl_version)
      {
        printf("Unable to load GL.\n");
        return;
      }
      printf("Loaded GL %d.%d\n", GLAD_VERSION_MAJOR(gl_version), GLAD_VERSION_MINOR(gl_version));

      if (!GLAD_GL_VERSION_3_3)
      {
        std::cerr << "OpenGL 3.3 is not supported" << std::endl;
        return;
      }
    }

  }

  void Initialize(const char *windowName)
  {
    CreateDisplay(windowName);
    renderThread = std::thread(RenderThread);
  }

  void Run(Engine::Data &engine)
  {
    while (XPending(display))
    {
      XEvent xev;
      XNextEvent(display, &xev);

      // Process only key events
      if (xev.type == KeyPress)
      {
        // Needed to map X11 KeyCode to native Linux
        int linuxKeyCode = xev.xkey.keycode - MIN_KEYCODE;
        Input::PushKeyQueue(static_cast<Input::KeyboardCode>(linuxKeyCode), &engine.keyEventData);
      }
    }
  }

  void Destroy()
  {
    quit = true;
    renderThread.join();

    // Cleanup
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, window);
    XFreeColormap(display, colormap);
    XCloseDisplay(display);
  }
}
