cmake_minimum_required(VERSION 3.26.0)
project(TEMP_ENGINE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set ( TEST_GAME_SRC
  "src/TestGame/main.cpp"
)
set ( TEST_GAME_2_SRC
  "src/TestGame2/main.cpp"
)
set ( ENGINE_HEADER
  "src/Engine/Components/Component.hpp"
  "src/Engine/Components/ComponentType.hpp"
  "src/Engine/Components/ComponentContainer.hpp"
  "src/Engine/Components/ComponentData.hpp"
  "src/Engine/Coordinator.hpp"
  "src/Engine/Engine.hpp"
  "src/Engine/Entity.hpp"
  "src/Engine/EntityManager.hpp"
  "src/Engine/Math.hpp"
  "src/Engine/Player.hpp"
  "src/Engine/Scene.hpp"
)
set ( ENGINE_SRC
  "src/Engine/Engine.cpp"
  "src/Engine/EntityManager.cpp"
  "src/Engine/Coordinator.cpp"
  "src/Engine/Components/ComponentContainer.cpp"
)
set ( TEST_GAME_HEADER
  "src/TestGame/TestLevel.hpp"
  "src/TestGame/Entry.hpp"
)
set ( TEST_GAME_2_HEADER
  "src/TestGame2/TestLevel.hpp"
  "src/TestGame2/Entry.hpp"
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${ENGINE_HEADER})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${ENGINE_SRC})

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TEST_GAME_HEADER})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TEST_GAME_SRC})

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TEST_GAME_2_HEADER})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TEST_GAME_2_SRC})

set(TARGET_TEST_GAME TestGame)
set(TARGET_TEST_GAME_2 TestGame2)
set(TARGET_ENGINE TempEngine)

add_library(${TARGET_ENGINE} ${ENGINE_HEADER} ${ENGINE_SRC})
add_executable(${TARGET_TEST_GAME} ${TEST_GAME_SRC} ${TEST_GAME_HEADER})
add_executable(${TARGET_TEST_GAME_2} ${TEST_GAME_2_SRC} ${TEST_GAME_2_HEADER})

set_target_properties(${TARGET_ENGINE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${TARGET_ENGINE} PROPERTIES PUBLIC_HEADER ${ENGINE_HEADER})

include_directories(src src/Engine src/Engine/Components src/TestGame src/TestGame2)

target_link_libraries(${TARGET_TEST_GAME} ${TARGET_ENGINE})
target_link_libraries(${TARGET_TEST_GAME_2} ${TARGET_ENGINE})

target_compile_options(${TARGET_TEST_GAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
target_compile_options(${TARGET_TEST_GAME_2} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
