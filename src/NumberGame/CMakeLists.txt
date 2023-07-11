set(NUMBER_GAME_SRC
  Components/ComponentContainer.cpp
  Coordinator.cpp
  GameLevel.cpp
  Grid.cpp
  MainMenuLevel.cpp
  Scene.cpp
  main.cpp
)

add_executable(${TARGET_NUMBER_GAME} ${NUMBER_GAME_SRC})

set(NUMBER_GAME_INCLUDE_DIR
  Components
)

add_dependencies(${TARGET_NUMBER_GAME} ${TARGET_ENGINE})
target_include_directories(${TARGET_NUMBER_GAME} PRIVATE ${NUMBER_GAME_INCLUDE_DIR} ${SRC_DIR})
target_link_libraries(${TARGET_NUMBER_GAME} PRIVATE ${TARGET_ENGINE})

if(APPLE)
  set_target_properties(${TARGET_NUMBER_GAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic")
  set_target_properties(${TARGET_NUMBER_GAME} PROPERTIES XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "32SB9VAM46")
  set_target_properties(${TARGET_NUMBER_GAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development")
endif()

set_target_properties(${TARGET_NUMBER_GAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NumberGame"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NumberGame"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NumberGame"
)

target_compile_options(${TARGET_NUMBER_GAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
