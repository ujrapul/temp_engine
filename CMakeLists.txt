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

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
if (SANITIZE_THREAD STREQUAL "ON")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif()
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_VERBOSE_BUILD ON)
if (USE_CLANG_TIDY STREQUAL "ON")
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -checks='-*,performance-*,modernize-*,-modernize-use-trailing-return-type,-modernize-avoid-c-arrays,-clang-analyzer-cplusplus.NewDeleteLeaks,';
    -fix;)
endif()

set(TARGET_ENGINE TempEngine)
set(TARGET_ENGINE_EDITOR TempEngineEditor)
set(TARGET_EDITOR Editor)
set(TARGET_NUMBER_GAME NumberGame)
set(TARGET_ENGINE_UNIT_TESTS UT_Engine)

file(GLOB_RECURSE SOURCE_LIST
  "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.glsl"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.h"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.hpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.level"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.lua"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.m"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.mm"
  "${CMAKE_CURRENT_LIST_DIR}/src/*.txt"
  "*.sh"
  "*.md"
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${SOURCE_LIST})
add_custom_target(Development SOURCES ${SOURCE_LIST})

add_subdirectory(src/Engine)
add_subdirectory(src/Editor)
add_subdirectory(src/NumberGame)
add_subdirectory(src/UT_Engine)
