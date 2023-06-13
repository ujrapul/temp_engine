#include "X11Render.hpp"
#include "Input.hpp"
#include "ImageLoader.hpp"
#include "gl.h"
#include "glx.h"
#include "OpenGLWrapper.hpp"
#include "RenderUtils.hpp"
#include "Drawable.hpp"
#include "Scene.hpp"
#include <chrono>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define MIN_KEYCODE 8

namespace Temp::Render
{
  namespace
  {
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

    void RenderThread(Engine::Data &engine)
    {
      // Make the OpenGL context current for the rendering thread
      glXMakeCurrent(display, window, context);

      glEnable(GL_DEPTH_TEST);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      Font::LoadFont();

      auto start = std::chrono::high_resolution_clock::now();

      // Cube mesh;
      // Initialize(mesh);

      float time = 0;

      // Main rendering loop
      while (!quit)
      {
        auto stop = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
        start = stop;

        time += deltaTime;

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: Observe if objects get constructed properly if this is left to run indefinitly
        // while (!engine.currentScene->renderConstructQueue.empty())
        // {
          Scene::DequeueRenderConstruct(engine.currentScene);
        // }

        if (engine.currentScene && engine.currentScene->state == Scene::State::RUN)
        {
          // std::cout << "GOT TO X11RENDER STATE RUN!" << std::endl;
          if (auto* drawableArray = Scene::GetComponentArray<Component::Type::DRAWABLE>(*engine.currentScene))
          {
            // std::cout << "GOT TO X11RENDER drawableArray!" << std::endl;

            for (auto i = 0; i < drawableArray->mapping.size; ++i)
            {
              // std::cout << "GOT TO X11RENDER DRAW!" << std::endl;
              Component::Drawable::Draw(&drawableArray->array[drawableArray->mapping.indexToEntity[i]]);
            }
          }
        }

        // Draw(mesh, time);

        glXSwapBuffers(display, window);

        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
      }

      // Clean up resources
      // Clean(mesh);
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
        None
      };

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

  void Initialize(const char *windowName, Engine::Data &engine)
  {
    CreateDisplay(windowName);
    renderThread = std::thread(RenderThread, std::ref(engine));
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
