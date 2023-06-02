#include "Engine.hpp"
#include "Scene.hpp"
#include "Render/glad/gl.h"
#include <chrono>
#include <thread>
#include <condition_variable>
#ifdef __linux__
#include "Render/glad/glx.h"
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define MIN_KEYCODE 8
#endif


namespace Temp::Engine
{
  namespace
  {
    float deltaTime{};

#ifdef __linux__
    // X11 Display and Window variables
    Display *display{nullptr};
    Window window{};

    // GLX variables
    GLXContext context{};
    XVisualInfo *visualInfo{};
    Colormap colormap{};

    constexpr int window_width = 800, window_height = 480;

    bool quit = false;

    void RenderThread()
    {
      // Make the OpenGL context current for the rendering thread
      glXMakeCurrent(display, window, context);

      auto start = std::chrono::high_resolution_clock::now();

      // Main rendering loop
      while (!quit)
      {
        auto stop = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(stop - start).count();
        start = stop;

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Perform your OpenGL rendering here

        glXSwapBuffers(display, window);

        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
      }
    }

    void CreateDisplay()
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
      window = XCreateWindow(display, rootWindow, 0, 0, 800, 600, 0,
                             visualInfo->depth, InputOutput, visualInfo->visual,
                             CWColormap | CWEventMask, &windowAttribs);

      // Set the window title
      XStoreName(display, window, "OpenGL with X11");

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
    }

#endif
  }

  void Run(Engine::Data &engine)
  {
    CreateDisplay();
    Scene::Data *currentScene = engine.scenes.front();

#ifndef __linux__
    std::thread inputThread(Input::HandleThread, std::ref(engine.keyEventData));
    inputThread.detach();
#endif
    std::thread renderThread(RenderThread);

    while (currentScene)
    {
      while (XPending(display))
      {
        XEvent xev;
        XNextEvent(display, &xev);

        // Process only key events
        if (xev.type == KeyPress) {
          // Needed to map X11 KeyCode to native Linux
          int linuxKeyCode = xev.xkey.keycode - MIN_KEYCODE;
          Input::PushKeyQueue(static_cast<Temp::Input::KeyboardCode>(linuxKeyCode), &engine.keyEventData);
        }
      }
      
      switch (currentScene->state)
      {
      case Scene::State::ENTER:
        if (currentScene->Construct)
          currentScene->Construct(currentScene);
        currentScene->state = Scene::State::RUN;
        break;
      case Scene::State::RUN:
        if (currentScene->Update)
          currentScene->Update(currentScene, deltaTime);
        break;
      case Scene::State::LEAVE:
        if (currentScene->Destruct)
          currentScene->Destruct(currentScene);
        currentScene->state = Scene::State::ENTER;
        currentScene = currentScene->nextScene;
        if (currentScene)
          currentScene->state = Scene::State::ENTER;
        break;
      default:
        break;
      }

      Input::Process(engine.keyEventData);
    }

    quit = true;
    renderThread.join();
  }

  void Destroy(Engine::Data &engine)
  {
    // For now the games should handle clean up of scenes
    for (Scene::Data *scene : engine.scenes)
    {
      delete scene;
    }
    engine.scenes.clear();

    // Cleanup
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, window);
    XFreeColormap(display, colormap);
    XCloseDisplay(display);
  }

  Engine::Data Construct()
  {
    Engine::Data out;
    out.keyEventData = std::move(Input::Construct());
    return out;
  }
}
