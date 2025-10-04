add_executable(main main.cpp window.cpp 
  ${IMGUI_SRC}/imgui.cpp ${IMGUI_SRC}/imgui_draw.cpp ${IMGUI_SRC}/imgui_tables.cpp ${IMGUI_SRC}/imgui_widgets.cpp ${IMGUI_SRC}/imgui_demo.cpp
  ${IMGUI_SRC}/backends/imgui_impl_glfw.cpp ${IMGUI_SRC}/backends/imgui_impl_opengl3.cpp
  ${IMPLOT_SRC}/implot.cpp ${IMPLOT_SRC}/implot_items.cpp
)

target_include_directories(main PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/external/glad/include
  ${IMGUI_SRC}
  ${IMGUI_SRC}/backends
  ${IMPLOT_SRC}
)

target_link_libraries(main PUBLIC
  glad
  glfw
  dl
  OpenGL::OpenGL
)
