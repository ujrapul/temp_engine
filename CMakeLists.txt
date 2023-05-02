cmake_minimum_required(VERSION 3.26.0)
project(TEMP_ENGINE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("Components" ".")
set ( SRC
  "main.cpp"
)
set ( HEADER
  "Components/Component.hpp"
  "Components/ComponentType.hpp"
  "Components/ComponentContainer.hpp"
  "Components/ComponentData.hpp"
  "Coordinator.hpp"
  "Engine.hpp"
  "Entity.hpp"
  "EntityManager.hpp"
  "Math.hpp"
  "Player.hpp"
  "Scene.hpp"
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${SRC})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADER})

set(TARGET_NAME tempEngine)

add_executable(${TARGET_NAME} ${SRC} ${HEADER})

target_compile_options(${TARGET_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
