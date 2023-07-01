// DisplayManager.mm

#include "NSRender.hpp"
#include "Camera.hpp"
#include "FontLoader.hpp"
#include "OpenGLWrapper.hpp"
#include "Scene.hpp"
#include "Event.hpp"

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
//
//  void Resize(void *)
//  {
//    // TODO: Look into why glViewport here is causing the view to become larger
//    // Might be something to do with the NSOpenGLView that's taking care of the
//    // transformations for us.
//    //    glViewport(windowWidth / 2.f, windowHeight / 2.f, windowWidth, windowHeight);
//  }
}

void RenderThread()
{
  // Get the OpenGL context
  NSOpenGLContext *openGLContext = nsOpenGLView.openGLContext;
  // Make the OpenGL context current
  [openGLContext makeCurrentContext];
  
  Temp::Event::RenderSetup();

  // Main rendering loop
  while (!Temp::Event::EventData.renderQuit)
  {
    Temp::Event::RenderRun();
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
  Temp::Event::EventData.windowWidth = contentSize.width;
  Temp::Event::EventData.windowHeight = contentSize.height;
  
  Temp::Camera::UpdateCameraAspect(*engine, Temp::Event::EventData.windowWidth, Temp::Event::EventData.windowHeight);
//  Temp::Engine::EnqueueGlobalRender(*engine, Resize, nullptr);
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
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
    nsOpenGLView = [[[NSOpenGLView alloc] initWithFrame:windowRect pixelFormat:pixelFormat] autorelease];
    
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
    
    // Set the OpenGL context for the NSOpenGLView
    //  [self.openGLView setOpenGLContext:self.openGLView.openGLContext];
    
    // Run the main event loop
    //  [NSApp run];
    
    Temp::Engine::Start(*self.engine, self.windowName, Temp::Event::EventData.windowWidth, Temp::Event::EventData.windowHeight);
    
    // Run the custom event loop
    while (Temp::Engine::IsActive(*self.engine)) {
      Temp::Engine::Process(*self.engine);
      @autoreleasepool {
        NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
        if (event) {
          [NSApp sendEvent:event];
        } else {
        }
      }
    }
    
    Temp::Engine::Destroy(*self.engine);
  }
  [application terminate:nil];
}

- (void)windowWillClose:(NSNotification *)notification {
  if (Temp::Event::EventData.isInFullScreen) {
    Temp::Event::EventData.isInFullScreen = false;
    return;
  }

  // Terminate the application
  Temp::Engine::Destroy(*engine);
  [application terminate:nil];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
  // Remove the observer
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end

void CreateDisplay(const char *windowName, int windowX, int windowY, Temp::Engine::Data &engine) {
  @autoreleasepool {
    // Create the application instance
    application = [[NSApplication sharedApplication] autorelease];
    Temp::Event::EventData.windowWidth = windowX;
    Temp::Event::EventData.windowHeight = windowY;
    
    // Create the application delegate
    AppDelegate *appDelegate = [[[AppDelegate alloc] init] autorelease];
    appDelegate.engine = &engine;
    appDelegate.windowName = windowName;
    [application setDelegate:appDelegate];
    
    // Run the application
    [application run];
  }
}

namespace Temp::Render
{
  void Initialize(const char *windowName, int windowX, int windowY, Temp::Engine::Data &engine)
  {
    Temp::Camera::UpdateCameraAspect(engine, windowX, windowY);
    Temp::Event::EventData.renderThread = std::thread(RenderThread);
  }
  
  bool IsInitialized()
  {
    return Temp::Event::EventData.renderInitialized;
  }
  
  void Run(Temp::Engine::Data &engine, const char *windowName, int windowX, int windowY)
  {
    CreateDisplay(windowName, windowX, windowY, engine);
  }
  
  // Process Events
  void Run(Engine::Data &engine)
  {
    
  }
  
  void Destroy()
  {
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
