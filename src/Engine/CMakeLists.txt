set ( ENGINE_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Components
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/LuaJIT/src
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/stbimage
  ${CMAKE_CURRENT_SOURCE_DIR}/Entity
  ${CMAKE_CURRENT_SOURCE_DIR}/Render
  ${CMAKE_CURRENT_SOURCE_DIR}/Render/OpenGL
  ${CMAKE_CURRENT_SOURCE_DIR}/Render/OpenGL/Shaders
  ${CMAKE_CURRENT_SOURCE_DIR}/Render/glad
  ${CMAKE_CURRENT_SOURCE_DIR}/Scene
)
set ( ENGINE_SRC
  Camera.cpp
  Components/ComponentContainer.cpp
  Components/Drawable.cpp
  Components/Luable.cpp
  Coordinator.cpp
  Dependencies/stbimage/stb_image.cpp # TODO: When you become more knowledgable try writing your own image loading system
  Engine.cpp
  EngineUtils.cpp
  Entity/EntityManager.cpp
  Entity/TextBox.cpp
  FontLoader.cpp
  Input.cpp
  Render/OpenGL/OpenGLWrapper.cpp
  Render/glad/gl.c
  Scene/LevelParser.cpp
  Scene/Scene.cpp
  Scene/SceneObject.cpp
)
if (WIN32)
  list (APPEND ENGINE_SRC
    Render/glad/wgl.c
  )
elseif (LINUX)
  list (APPEND ENGINE_SRC
    Render/OpenGL/X11Render.cpp
    Render/OpenGL/imgui_impl_x11.cpp
    Render/glad/glx.c
  )
elseif (APPLE)
  list (APPEND ENGINE_SRC
    # Add your Objective-C and C++ source files
    Render/OpenGL/NSRender.mm
  )
  # Set the compiler flags for Objective-C and C++
  #set_source_files_properties(${ENGINE_SRC} PROPERTIES
  #  COMPILE_FLAGS "-x objective-c++"
endif()

add_library(${TARGET_ENGINE} STATIC ${ENGINE_SRC})
target_include_directories(${TARGET_ENGINE} PUBLIC ${ENGINE_INCLUDE_DIRS})
set_target_properties(${TARGET_ENGINE} PROPERTIES LINKER_LANGUAGE CXX)
include_directories(
  ${OPENGL_INCLUDE_DIRS}
  Components
  Dependencies
  Dependencies/LuaJIT/src
  Dependencies/stbimage
  Entity
  Render
  Render/OpenGL
  Render/OpenGL/Shaders
  Render/glad
  Scene
)

target_link_libraries(${TARGET_ENGINE} PRIVATE ${OPENGL_LIBRARIES})
if (LINUX)
  target_link_libraries(${TARGET_ENGINE} PRIVATE X11)
endif()

# Set the path to LuaJIT source directory
set(LUAJIT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/LuaJIT")  # Update the path to the LuaJIT source directory

# Set the path to LuaJIT build directory
set(LUAJIT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/LuaJIT")  # Update the path to the desired LuaJIT build directory

# Configure and build LuaJIT using Makefile
if(UNIX)
  set (LUA_BUILD_CMD make)
else()
  set (LUA_BUILD_CMD msvcbuild.bat)
endif()
ExternalProject_Add(
  LuaJIT
  PREFIX ${LUAJIT_BINARY_DIR}
  SOURCE_DIR ${LUAJIT_SOURCE_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${LUA_BUILD_CMD}
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE true
  BUILD_BYPRODUCTS ${LUAJIT_SOURCE_DIR}/src/libluajit.a
)

# Include LuaJIT headers
include_directories(SYSTEM ${LUAJIT_BINARY_DIR}/src)  # Update the path to the LuaJIT headers
set_target_properties(LuaJIT PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:LuaJIT,INTERFACE_INCLUDE_DIRECTORIES>)

# Link with LuaJIT library
add_dependencies(${TARGET_ENGINE} LuaJIT)
if(UNIX)
  target_link_libraries(${TARGET_ENGINE} PRIVATE ${LUAJIT_SOURCE_DIR}/src/libluajit.a)
else()
  target_link_libraries(${TARGET_ENGINE} PRIVATE ${LUAJIT_SOURCE_DIR}/src/libluajit.lib)
endif()

# TODO: When you become more knowledgable try writing your own font loading system
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/freetype)
target_include_directories(${TARGET_ENGINE} SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/freetype/include)
set_target_properties(freetype PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:freetype,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(${TARGET_ENGINE} PRIVATE freetype)

# Add the ImGui library
add_library(imgui
  OBJECT
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_widgets.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_glfw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_opengl3.cpp
)
set_target_properties(
  imgui
  PROPERTIES
    CXX_CLANG_TIDY ""
)

target_include_directories(imgui
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/backends
)

target_link_libraries(${TARGET_ENGINE} PRIVATE imgui)

if(APPLE)
  target_link_libraries(${TARGET_ENGINE} PRIVATE
  "-framework CoreFoundation"
  "-framework CoreGraphics"
  "-framework AppKit"
  "-framework OpenGL"
  "-framework Cocoa"
  )
  target_compile_definitions(${TARGET_ENGINE} PUBLIC GL_SILENCE_DEPRECATION)
endif()

set_target_properties(${TARGET_ENGINE} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Engine"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Engine"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Engine"
)

target_compile_options(${TARGET_ENGINE} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

