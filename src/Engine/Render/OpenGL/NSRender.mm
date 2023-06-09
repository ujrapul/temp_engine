// DisplayManager.mm

#include "NSRender.hpp"
#include "Camera.hpp"
#include "FontLoader.hpp"
#include "OpenGLWrapper.hpp"
#include "Scene.hpp"
#include "Event.hpp"

#ifdef EDITOR
#include "Editor.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_osx.h"
#endif

#import <Foundation/Foundation.h>
#import <OpenGL/gl3.h>
#import <OpenGL/OpenGL.h>
#import <OpenGL/CGLContext.h>
#import <Cocoa/Cocoa.h>
#include <thread>

namespace
{
  NSOpenGLView* nsOpenGLView{nullptr};
  NSApplication *application{nullptr};
  NSWindow *window{nullptr};
  Temp::Engine::Data* engine{nullptr};
  std::mutex imguiMtx{};
  //
  //  void Resize(void *)
  //  {
  //    // TODO: Look into why glViewport here is causing the view to become larger
  //    // Might be something to do with the NSOpenGLView that's taking care of the
  //    // transformations for us.
  //    //    glViewport(windowWidth / 2.f, windowHeight / 2.f, windowWidth, windowHeight);
  //  }
#ifdef EDITOR
  void RenderImGui()
  {
    std::lock_guard<std::mutex> lock(imguiMtx);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    
    Temp::Editor::RunImGUI();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
#endif
  
  void RunMainLoop(const char* windowName) {
    @autoreleasepool {
      [application stop:nil];

      // Set the OpenGL context for the window
      engine = &Temp::Engine::engine;
      
      {
        std::lock_guard<std::mutex> eventLock(Temp::Event::EventData.mtx);
        Temp::Engine::Start(*engine, windowName, Temp::Event::EventData.windowWidth, Temp::Event::EventData.windowHeight);
      }

  #ifdef EDITOR
      {
        std::lock_guard<std::mutex> lock(imguiMtx);
        // Setup Dear ImGui context
        // FIXME: This example doesn't have proper cleanup...
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();
        
        // Setup Platform/Renderer backends
        ImGui_ImplOSX_Init(nsOpenGLView);
      }
  #endif
      
      // Run the custom event loop
      while (Temp::Engine::IsActive(*engine)) {
        Temp::Engine::Process(*engine);
  #ifdef EDITOR
        {
          std::lock_guard<std::mutex> lock(imguiMtx);
          ImGui_ImplOSX_NewFrame(nsOpenGLView);
        }
  #endif
        @autoreleasepool {
          NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
          if (event) {
            [NSApp sendEvent:event];
          }
        }
      }
    }
  #ifdef EDITOR
    ImGui_ImplOSX_Shutdown();
  #endif
    Temp::Engine::Destroy(*engine);
  }
}

@interface TempOpenGLView : NSOpenGLView
@end

@implementation TempOpenGLView
- (void)keyDown:(NSEvent *)theEvent {
}
// Needed to remove 'ding'/'beep' sound when pressing key
- (BOOL)acceptsFirstResponder {
  return YES;
}
@end

void RenderThread()
{
  // Get the OpenGL context
  NSOpenGLContext *openGLContext = nsOpenGLView.openGLContext;
  // Make the OpenGL context current
  [openGLContext makeCurrentContext];
  
  Temp::Event::RenderSetup();
  
#ifdef EDITOR
  {
    std::lock_guard<std::mutex> lock(imguiMtx);
    ImGui_ImplOpenGL3_Init();
  }
#endif
  
  // Main rendering loop
  while (!Temp::Event::EventData.renderQuit)
  {
    Temp::Event::RenderRun();
#ifdef EDITOR
    RenderImGui();
#endif
    // Swap buffers
    [openGLContext flushBuffer];
  }
  
  // TODO: Clean up resources
  Temp::Event::RenderClean();
}

@interface WindowDelegate : NSObject <NSWindowDelegate>
@end

@implementation WindowDelegate

- (void)windowDidResize:(NSNotification *)notification {
  // Perform custom handling for resize event
  NSRect contentRect = [window contentRectForFrameRect:window.frame];
  NSSize contentSize = contentRect.size;
  std::lock_guard<std::mutex> eventLock(Temp::Event::EventData.mtx);
  Temp::Event::EventData.windowWidth = contentSize.width;
  Temp::Event::EventData.windowHeight = contentSize.height;
  
  Temp::Camera::UpdateCameraAspect(*engine, Temp::Event::EventData.windowWidth, Temp::Event::EventData.windowHeight);
  //  Temp::Engine::EnqueueGlobalRender(*engine, Resize, nullptr);
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
  std::lock_guard<std::mutex> eventLock(Temp::Event::EventData.mtx);
  Temp::Event::EventData.isInFullScreen = true;
}

@end

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic) const char* windowName;
@property (nonatomic) Temp::Engine::Data* engine;
//@property (nonatomic, strong) MyOpenGLView *openGLView;
@property bool shouldTerminate;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  @autoreleasepool {
    // Set the OpenGL context for the window
    self.shouldTerminate = false;
    engine = self.engine;
    
    NSOpenGLPixelFormatAttribute attributes[] = {
      NSOpenGLPFAAccelerated,
      NSOpenGLPFADoubleBuffer,
      NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
      NSOpenGLPFADepthSize, 24,
      0
    };
    
    // Set the activation policy to regular
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    // Activate the application
    [NSApp activateIgnoringOtherApps:YES];
    
    // Add observer for the window close notification
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:nil];
    
    NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
    //  NSOpenGLContext *openGLContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
    
    // Create the OpenGL view
    NSRect windowRect = NSMakeRect(0, 0, Temp::Event::EventData.windowWidth, Temp::Event::EventData.windowHeight);
    nsOpenGLView = [[[TempOpenGLView alloc] initWithFrame:windowRect pixelFormat:pixelFormat] autorelease];
    
    // Create the window and set the content view
    window = [[[NSWindow alloc] initWithContentRect:windowRect styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable backing:NSBackingStoreBuffered defer:NO] autorelease];
    [window setTitle:@"OpenGL Window"];
    [window setContentView:nsOpenGLView];
    [window makeKeyAndOrderFront:nil];
    
    auto *windowDelegate = [[[WindowDelegate alloc] init] autorelease];
    window.delegate = windowDelegate;
    
    // Calculate the center coordinates of the screen
    NSScreen *screen = [NSScreen mainScreen];
    NSRect screenFrame = [screen visibleFrame];
    NSPoint centerPoint = NSMakePoint(NSMidX(screenFrame), NSMidY(screenFrame));
    
    // Calculate the origin coordinates for the window to be centered
    NSRect centeredFrame = NSMakeRect(centerPoint.x - windowRect.size.width / 2,
                                      centerPoint.y - windowRect.size.height / 2,
                                      windowRect.size.width,
                                      windowRect.size.height);
    
    // Set the window's frame to be centered on the screen
    [window setFrame:centeredFrame display:YES];
  }
  [application stop:nil];
}

- (void)windowWillClose:(NSNotification *)notification {
  std::lock_guard<std::mutex> eventLock(Temp::Event::EventData.mtx);
  if (Temp::Event::EventData.isInFullScreen) {
    Temp::Event::EventData.isInFullScreen = false;
    return;
  }
  
  Temp::Engine::Quit(*self.engine);
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
  // Remove the observer
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
  return YES;
}

@end

void CreateDisplay(const char *windowName, int windowX, int windowY, Temp::Engine::Data &engine) {
  @autoreleasepool {
    // Create the application instance
    application = [[NSApplication sharedApplication] autorelease];
    {
      std::lock_guard<std::mutex> eventLock(Temp::Event::EventData.mtx);
      Temp::Event::EventData.windowWidth = windowX;
      Temp::Event::EventData.windowHeight = windowY;
    }
    
    // Create the application delegate
    AppDelegate *appDelegate = [[[AppDelegate alloc] init] autorelease];
    appDelegate.engine = &engine;
    appDelegate.windowName = windowName;
    [application setDelegate:appDelegate];
    
    // Run the application
    if (![[NSRunningApplication currentApplication] isFinishedLaunching])
      [application run];
    
    RunMainLoop(windowName);
  }
}

namespace Temp::Render
{
  void Initialize(const char */*windowName*/, int windowX, int windowY, Temp::Engine::Data &engine)
  {
    Temp::Camera::UpdateCameraAspect(engine, windowX, windowY);
    Temp::Event::EventData.renderThread = std::thread(RenderThread);
  }
  
  bool IsInitialized()
  {
    return Temp::Event::RenderInitialized();
  }
  
  void Run(Temp::Engine::Data &engine, const char *windowName, int windowX, int windowY)
  {
    CreateDisplay(windowName, windowX, windowY, engine);
  }
  
  // Process Events
  void Run(Engine::Data &/*engine*/)
  {
    
  }
  
  void Destroy()
  {
    std::lock_guard<std::mutex> eventLock(Temp::Event::EventData.mtx);
    Temp::Event::EventData.renderQuit = true;
    Temp::Event::EventData.renderThread.join();
  }
  
  Math::Vec2f GetWindowOrigin()
  {
    __block Math::Vec2f origin;
    dispatch_sync(dispatch_get_main_queue(), ^{
      NSRect windowFrame = [window convertRectToScreen:[window frame]];
      CGFloat windowX = NSMinX(windowFrame);
      CGFloat windowY = NSMinY(windowFrame);
      origin = {(float)windowX, (float)windowY};
    });
    return origin;
  }
  
  Math::Vec2f GetWindowSize()
  {
    __block Math::Vec2f size;
    dispatch_sync(dispatch_get_main_queue(), ^{
      NSRect contentRect = [window contentRectForFrameRect:window.frame];
      NSSize contentSize = contentRect.size;
      CGFloat width = contentSize.width;
      CGFloat height = contentSize.height;
      size = {(float)width, (float)height};
    });
    return size;
  }
  
  Math::Vec2f GetScreenSize()
  {
    __block Math::Vec2f size;
    dispatch_sync(dispatch_get_main_queue(), ^{
      NSSize screenSize = [[NSScreen mainScreen] frame].size;
      CGFloat screenWidth = screenSize.width;
      CGFloat screenHeight = screenSize.height;
      size = {(float)screenWidth, (float)screenHeight};
    });
    return size;
  }
}
