set(EDITOR_SRC
  EditorLevel.cpp
  main.cpp
)

add_executable(${TARGET_EDITOR} ${EDITOR_SRC})

set(EDITOR_INCLUDE_DIR
  .
)

add_dependencies(${TARGET_EDITOR} ${TARGET_ENGINE_EDITOR})
target_include_directories(${TARGET_EDITOR} PRIVATE ${EDITOR_INCLUDE_DIR} ${SRC_DIR})
target_link_libraries(${TARGET_EDITOR} PRIVATE ${TARGET_ENGINE_EDITOR})

if(APPLE)
  set_target_properties(${TARGET_EDITOR} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic")
  set_target_properties(${TARGET_EDITOR} PROPERTIES XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "32SB9VAM46")
  set_target_properties(${TARGET_EDITOR} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development")
endif()

set_target_properties(${TARGET_EDITOR} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${TARGET_EDITOR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${TARGET_EDITOR}"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${TARGET_EDITOR}"
)

# add_custom_command(
#   TARGET ${TARGET_EDITOR} POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_if_different
#   ${CMAKE_BINARY_DIR}/Engine/*
#   $<TARGET_FILE_DIR:${TARGET_EDITOR}>
#   COMMENT "Copying Target Engine Library to ${TARGET_EDITOR} output directory"
# )

target_compile_options(${TARGET_EDITOR} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
