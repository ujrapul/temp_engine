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
#include "glx.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
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
    // X11 Display and Window variables
    Display *display{nullptr};
    Window window{};

    // GLX variables
    GLXContext context{};
    XVisualInfo *visualInfo{};
    Colormap colormap{};

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
      glXMakeCurrent(display, window, context);

      // Vertical Sync | Set to 1 to Enable | 0 to disable
      glXSwapIntervalEXT(display, window, 0);

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

        // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Avoid using the Render Queue for real-time updates to avoid flickering!
        Engine::DequeueGlobalRender(engine);
        if (engine.currentScene) [[likely]]
        {
          Temp::Scene::Draw(*engine.currentScene);
        }

        glXSwapBuffers(display, window);
      }

      // TODO: Clean up resources
    }

    void CreateDisplay(const char *windowName, int windowX, int windowY)
    {
      windowWidth = windowX;
      windowHeight = windowY;

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
      windowAttribs.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask | PointerMotionMask | ButtonPressMask;
      colormap = windowAttribs.colormap;

      // Create the window
      window = XCreateWindow(display, rootWindow, 0, 0, windowX, windowY, 0,
                             visualInfo->depth, InputOutput, visualInfo->visual,
                             CWColormap | CWEventMask, &windowAttribs);

      // Set the window title
      XStoreName(display, window, windowName);

      // Set the WM_DELETE_WINDOW protocol on the window
      Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
      XSetWMProtocols(display, window, &wmDeleteWindow, 1);

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

  // TODO: Try to thread this
  void Run(Engine::Data &engine)
  {
    while (XPending(display))
    {
      XEvent xev;
      XNextEvent(display, &xev);

      switch (xev.type)
      {
      case KeyPress:
      {
        // Needed to map X11 KeyCode to native Linux
        int linuxKeyCode = xev.xkey.keycode - MIN_KEYCODE;
        Input::PushKeyQueue(static_cast<Input::KeyboardCode>(linuxKeyCode), &engine.keyEventData);
      }
      break;
      case ClientMessage:
      {
        // Window close check
        Atom wmProtocols = XInternAtom(display, "WM_PROTOCOLS", False);
        Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
        if (xev.type == ClientMessage && xev.xclient.message_type == wmProtocols)
        {
          Atom actualAtom;
          int actualFormat;
          unsigned long numItems, bytesAfter;
          Atom *data = nullptr;
          if (XGetWindowProperty(display, xev.xclient.window, wmProtocols, 0, 1, False, AnyPropertyType,
                                 &actualAtom, &actualFormat, &numItems, &bytesAfter, (unsigned char **)&data) == Success)
          {
            if (numItems > 0 && data[0] == wmDeleteWindow)
            {
              Engine::Quit(engine);
            }
            XFree(data);
          }
        }
      }
      break;
      case ConfigureNotify:
      {
        XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);

        int currentWidth = windowAttributes.width;
        int currentHeight = windowAttributes.height;

        if (currentWidth != windowWidth || currentHeight != windowHeight)
        {
          Camera::UpdateCameraAspect(engine, currentWidth, currentHeight);
          windowWidth = currentWidth;
          windowHeight = currentHeight;

          Engine::EnqueueGlobalRender(engine, Resize, nullptr);
        }
      }
      break;
      case MotionNotify:
      {
        XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);

        int mouseX = xev.xbutton.x - windowAttributes.x;
        int mouseY = xev.xbutton.y - windowAttributes.y;

        auto *scene = engine.currentScene;
        if (scene->state == Scene::State::RUN)
        {
          auto &hoverableArray = Scene::GetComponentArray<Component::Type::HOVERABLE>(*scene);
          for (size_t i = 0; i < hoverableArray.mapping.size; ++i)
          {
            auto &hoverable = hoverableArray.array[i];
            if (Component::Hoverable::IsInside(hoverable, mouseX, mouseY))
            {
              Component::Hoverable::HoverableEnter(*scene, hoverable);
            }
            else
            {
              Component::Hoverable::HoverableLeave(*scene, hoverable);
            }
          }
        }
      }
      break;
      case ButtonPress:
      {
        XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, window, &windowAttributes);

        int mouseX = xev.xbutton.x - windowAttributes.x;
        int mouseY = xev.xbutton.y - windowAttributes.y;
        int button = xev.xbutton.button;

        Window childWindow;

        auto *scene = engine.currentScene;
        if (scene->state == Scene::State::RUN)
        {
          auto &hoverableArray = Scene::GetComponentArray<Component::Type::HOVERABLE>(*scene);
          for (size_t i = 0; i < hoverableArray.mapping.size; ++i)
          {
            auto &hoverable = hoverableArray.array[i];
            if (Component::Hoverable::IsInside(hoverable, mouseX, mouseY))
            {
              hoverable.Click(*scene, hoverable);
            }
          }
        }
      }
      break;
      default:
        break;
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
