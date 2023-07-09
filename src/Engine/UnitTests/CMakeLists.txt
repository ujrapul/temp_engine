set ( ENGINE_UT_SOURCE
  UT_Main.cpp
)

add_executable(${TARGET_ENGINE_UNIT_TESTS} ${ENGINE_UT_SOURCE})

add_dependencies(${TARGET_ENGINE_UNIT_TESTS} ${TARGET_ENGINE})
target_include_directories(${TARGET_NUMBER_GAME} PRIVATE ${SRC_DIR})
target_link_libraries(${TARGET_ENGINE_UNIT_TESTS} ${TARGET_ENGINE})

if(APPLE)
  target_compile_definitions(${TARGET_ENGINE_UNIT_TESTS} PUBLIC GL_SILENCE_DEPRECATION)
endif()

set_target_properties(${TARGET_ENGINE_UNIT_TESTS} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/UnitTests"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/UnitTests"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/UnitTests"
)

add_custom_command(
  TARGET ${TARGET_ENGINE_UNIT_TESTS} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/Engine/*
    $<TARGET_FILE_DIR:${TARGET_ENGINE_UNIT_TESTS}>
  COMMENT "Copying Target Engine Library to ${TARGET_ENGINE_UNIT_TESTS} output directory"
)

target_compile_options(${TARGET_ENGINE_UNIT_TESTS} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

