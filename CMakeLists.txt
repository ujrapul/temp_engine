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
  "Components/DataManipulator.hpp"
  "Components/Position2D.hpp"
  "Components/Transform.hpp"
  "Engine.hpp"
  "Entity.hpp"
  "EntityManager.hpp"
  "Math.hpp"
  "Player.hpp"
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${SRC})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${HEADER})

add_executable(tempEngine ${SRC} ${HEADER})
