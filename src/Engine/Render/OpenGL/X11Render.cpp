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
    bool initialized = false;

    void RenderThread(Engine::Data &engine)
    {
      // Make the OpenGL context current for the rendering thread
      glXMakeCurrent(display, window, context);

      glEnable(GL_DEPTH_TEST);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      Font::LoadFont();
      OpenGLWrapper::LoadShaders();

      initialized = true;

      // Main rendering loop
      while (!quit)
      {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!engine.currentScene)
        {
          continue;
        }
        engine.currentScene->Draw(engine.currentScene);

        glXSwapBuffers(display, window);
      }

      // TODO: Clean up resources
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

  void Initialize(const char *windowName, Engine::Data &engine)
  {
    CreateDisplay(windowName);
    renderThread = std::thread(RenderThread, std::ref(engine));
  }

  bool IsInitialized()
  {
    return initialized;
  }

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
