add_executable(test test.c)

target_link_libraries(test PUBLIC
  glad
  glfw
)

add_executable(main main.c shader.c)

target_include_directories(main PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_link_libraries(main PUBLIC
  glad
  glfw
)
