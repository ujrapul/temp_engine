cmake_minimum_required(VERSION 3.26.0)
project(TEMP_ENGINE)

include(ExternalProject)

add_compile_definitions("$<$<CONFIG:DEBUG>:-DDEBUG>")

if (UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

# Find the OpenGL package
find_package(OpenGL REQUIRED)
if (LINUX)
  find_package(X11 REQUIRED)
endif()

if (APPLE)
  enable_language(OBJCXX)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_VERBOSE_BUILD ON)

set ( TEST_GAME_SRC
  src/TestGame/main.cpp
)
set ( TEST_GAME_2_SRC
  src/TestGame2/main.cpp
)
set ( NUMBER_GAME_SRC
  src/NumberGame/Components/ComponentContainer.cpp
  src/NumberGame/Coordinator.cpp
  src/NumberGame/GameLevel.cpp
  src/NumberGame/MainMenuLevel.cpp
  src/NumberGame/Grid.cpp
  src/NumberGame/Scene.cpp
  src/NumberGame/main.cpp
)
set ( ENGINE_HEADER
  src/Engine/Camera.hpp
  src/Engine/Components/Component.hpp
  src/Engine/Components/ComponentContainer.hpp
  src/Engine/Components/ComponentData.hpp
  src/Engine/Components/ComponentType.hpp
  src/Engine/Components/Drawable.hpp
  src/Engine/Components/Hoverable.hpp
  src/Engine/Components/Luable.hpp
  src/Engine/Coordinator.hpp
  src/Engine/Engine.hpp
  src/Engine/EngineUtils.hpp
  src/Engine/Entity.hpp
  src/Engine/EntityManager.hpp
  src/Engine/Event.hpp
  src/Engine/FontLoader.hpp
  src/Engine/ImageLoader.hpp
  src/Engine/Input.hpp
  src/Engine/Logger.hpp
  src/Engine/Math.hpp
  src/Engine/Player.hpp
  src/Engine/Render/OpenGL/OpenGLWrapper.hpp
  src/Engine/Render/RenderUtils.hpp
  src/Engine/Render/glad/gl.h
  src/Engine/Render/glad/khrplatform.h
  src/Engine/Scene.hpp
  src/Engine/TextButton.hpp
  src/Engine/TextBox.hpp
  src/Engine/Dependencies/stbimage/stb_image.h # TODO: When you become more knowledgable try writing your own image loading system
)
set ( ENGINE_UT_HEADER
  src/Engine/UnitTests/UT_Common.hpp
  src/Engine/UnitTests/UT_ComponentData.hpp
  src/Engine/UnitTests/UT_ComponentContainer.hpp
  src/Engine/UnitTests/UT_Event.hpp
  src/Engine/UnitTests/UT_Hoverable.hpp
  src/Engine/UnitTests/UT_Math.hpp
)
set ( ENGINE_UT_SOURCE
  src/Engine/UnitTests/UT_Main.cpp
)
if (WIN32)
  list (APPEND ENGINE_HEADER
    src/Engine/Render/glad/wgl.h
  )
elseif (LINUX)
  list (APPEND ENGINE_HEADER
    src/Engine/Render/glad/glx.h
    src/Engine/Render/OpenGL/X11Render.hpp
  )
elseif (APPLE)
  list (APPEND ENGINE_HEADER
    src/Engine/Render/OpenGL/NSRender.hpp
  )
endif()
set ( ENGINE_SRC
  src/Engine/Camera.cpp
  src/Engine/Components/ComponentContainer.cpp
  src/Engine/Components/Drawable.cpp
  src/Engine/Components/Luable.cpp
  src/Engine/Coordinator.cpp
  src/Engine/Engine.cpp
  src/Engine/EngineUtils.cpp
  src/Engine/EntityManager.cpp
  src/Engine/FontLoader.cpp
  src/Engine/Input.cpp
  src/Engine/Render/OpenGL/OpenGLWrapper.cpp
  src/Engine/Render/glad/gl.c
  src/Engine/Scene.cpp
  src/Engine/TextBox.cpp
  src/Engine/Dependencies/stbimage/stb_image.cpp # TODO: When you become more knowledgable try writing your own image loading system
)
if (WIN32)
  list (APPEND ENGINE_SRC
    src/Engine/Render/glad/wgl.c
  )
elseif (LINUX)
  list (APPEND ENGINE_SRC
    src/Engine/Render/glad/glx.c
    src/Engine/Render/OpenGL/X11Render.cpp
  )
elseif (APPLE)
  list (APPEND ENGINE_SRC
    # Add your Objective-C and C++ source files
    src/Engine/Render/OpenGL/NSRender.mm
  )
  # Set the compiler flags for Objective-C and C++
  #set_source_files_properties(${ENGINE_SRC} PROPERTIES
  #  COMPILE_FLAGS "-x objective-c++"
endif()
set ( TEST_GAME_HEADER
  src/TestGame/Entry.hpp
  src/TestGame/TestLevel.hpp
)
set ( TEST_GAME_2_HEADER
  src/TestGame2/Entry.hpp
  src/TestGame2/TestLevel.hpp
)
set ( NUMBER_GAME_HEADER
  src/NumberGame/Components/ComponentContainer.hpp
  src/NumberGame/Components/ComponentType.hpp
  src/NumberGame/Coordinator.hpp
  src/NumberGame/Entry.hpp
  src/NumberGame/GameLevel.hpp
  src/NumberGame/MainMenuLevel.hpp
  src/NumberGame/Grid.hpp
  src/NumberGame/Scene.hpp
)

set(TARGET_TEST_GAME TestGame)
set(TARGET_TEST_GAME_2 TestGame2)
set(TARGET_NUMBER_GAME NumberGame)
set(TARGET_ENGINE TempEngine)
set(TARGET_ENGINE_UNIT_TESTS TempEngineUT)

file(GLOB_RECURSE SOURCE_LIST
  "${CMAKE_CURRENT_LIST_DIR}/src/*.hpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.h"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.m"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.mm"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.glsl"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.lua"
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${SOURCE_LIST})
add_custom_target(Development SOURCES ${SOURCE_LIST})

add_library(${TARGET_ENGINE} ${ENGINE_HEADER} ${ENGINE_SRC})
#add_executable(${TARGET_TEST_GAME} ${TEST_GAME_SRC} ${TEST_GAME_HEADER})
#add_executable(${TARGET_TEST_GAME_2} ${TEST_GAME_2_SRC} ${TEST_GAME_2_HEADER})
add_executable(${TARGET_ENGINE_UNIT_TESTS} ${ENGINE_UT_HEADER} ${ENGINE_UT_SOURCE})
add_executable(${TARGET_NUMBER_GAME} ${NUMBER_GAME_SRC} ${NUMBER_GAME_HEADER})

set_target_properties(${TARGET_ENGINE} PROPERTIES LINKER_LANGUAGE CXX)

include_directories(
  ${OPENGL_INCLUDE_DIRS}
  src
  src/Engine
  src/Engine/Components
  src/Engine/Render
  src/Engine/Render/OpenGL
  src/Engine/Render/glad
  src/Engine/Render/OpenGL/Shaders
  src/Engine/Dependencies
  src/Engine/Dependencies/stbimage
  src/Engine/Dependencies/LuaJIT/src
  src/NumberGame
  src/NumberGame/Components
  src/TestGame
  src/TestGame2
)

target_link_libraries(${TARGET_ENGINE} PRIVATE ${OPENGL_LIBRARIES})
if (LINUX)
  target_link_libraries(${TARGET_ENGINE} PRIVATE X11)
endif()

# Set the path to LuaJIT source directory
set(LUAJIT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/Dependencies/LuaJIT")  # Update the path to the LuaJIT source directory

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
add_subdirectory(src/Engine/Dependencies/freetype)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/Dependencies/freetype/include)
set_target_properties(freetype PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:freetype,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(${TARGET_ENGINE} PRIVATE freetype)

add_dependencies(${TARGET_NUMBER_GAME} ${TARGET_ENGINE})
#target_link_libraries(${TARGET_TEST_GAME} ${TARGET_ENGINE})
#target_link_libraries(${TARGET_TEST_GAME_2} ${TARGET_ENGINE})
target_link_libraries(${TARGET_ENGINE_UNIT_TESTS} ${TARGET_ENGINE})
target_link_libraries(${TARGET_NUMBER_GAME} ${TARGET_ENGINE})

if(APPLE)
  target_link_libraries(${TARGET_ENGINE} PRIVATE
  "-framework CoreFoundation"
  "-framework CoreGraphics"
  "-framework AppKit"
  "-framework OpenGL"
  "-framework Cocoa"
  )
  set_target_properties(${TARGET_NUMBER_GAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic")
  set_target_properties(${TARGET_NUMBER_GAME} PROPERTIES XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "32SB9VAM46")
  set_target_properties(${TARGET_NUMBER_GAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development")
  target_compile_definitions(${TARGET_ENGINE_UNIT_TESTS} PUBLIC GL_SILENCE_DEPRECATION)
  target_compile_definitions(${TARGET_ENGINE} PUBLIC GL_SILENCE_DEPRECATION)
endif()

#target_compile_options(${TARGET_TEST_GAME} PRIVATE
#  $<$<CXX_COMPILER_ID:MSVC>:/w>
#  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
#)
#target_compile_options(${TARGET_TEST_GAME_2} PRIVATE
#  $<$<CXX_COMPILER_ID:MSVC>:/w>
#  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
#)
target_compile_options(${TARGET_ENGINE_UNIT_TESTS} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
target_compile_options(${TARGET_NUMBER_GAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
