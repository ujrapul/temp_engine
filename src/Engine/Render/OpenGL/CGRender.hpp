
#include "X11Render.hpp"

#include "Camera.hpp"
#include "Drawable.hpp"
#include "Hoverable.hpp"
#include "ImageLoader.hpp"
#include "Input.hpp"
#include "OpenGLWrapper.hpp"
#include "RenderUtils.hpp"
#include "Scene.hpp"
#include "gl.h"
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <unistd.h>
#define MIN_KEYCODE 8

namespace Temp::Render
{
  namespace
  {
//    // X11 Display and Window variables
//    Display *display{nullptr};
//    Window window{};
//
//    // GLX variables
//    GLXContext context{};
//    XVisualInfo *visualInfo{};
//    Colormap colormap{};

    std::thread renderThread{};

    bool quit = false;
    bool initialized = false;

    int windowWidth{};
    int windowHeight{};

    bool limitFps{false};
    float fps60{0.0166666 * 2};
    float fps30{0.0333333 * 2};

    void Resize(void *)
    {
      glViewport(0, 0, windowWidth, windowHeight);
    }

    void RenderThread(Engine::Data &engine)
    {
      // Make the OpenGL context current for the rendering thread
//      glXMakeCurrent(display, window, context);

      // Vertical Sync | Set to 1 to Enable | 0 to disable
//      glXSwapIntervalEXT(display, window, 0);

      glEnable(GL_DEPTH_TEST);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      Font::LoadFont();
      OpenGLWrapper::LoadShaders();

      initialized = true;

      Resize(nullptr);

      float time = 0;

      clock_t begin{clock()};
      // Main rendering loop
      while (!quit)
      {
        clock_t end{clock()};
        if (limitFps && ((float)(end - begin) / CLOCKS_PER_SEC) < fps60)
        {
          continue;
        }
        begin = clock();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Avoid using the Render Queue for real-time updates to avoid flickering!
        Engine::DequeueGlobalRender(engine);
        if (engine.currentScene) [[likely]]
        {
          Temp::Scene::Draw(engine.currentScene);
        }

//        glXSwapBuffers(display, window);
      }

      // TODO: Clean up resources
    }

    void CreateDisplay(const char *windowName, int windowX, int windowY)
    {
      windowWidth = windowX;
      windowHeight = windowY;
      
      // TODO:: Fill out
    }
  }

  void Initialize(const char *windowName, int windowX, int windowY, Engine::Data &engine)
  {
    CreateDisplay(windowName, windowX, windowY);
    Camera::UpdateCameraAspect(engine, windowX, windowY);
    renderThread = std::thread(RenderThread, std::ref(engine));
  }

  bool IsInitialized()
  {
    return initialized;
  }

  void Run(Engine::Data &engine)
  {

  }

  void Destroy()
  {
    quit = true;
    renderThread.join();

    // Cleanup

  }
}
