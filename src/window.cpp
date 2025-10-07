#include "window.hpp"
#include <GLFW/glfw3.h>
#include <print>
#include <cstdlib>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void initWindow(GLFWwindow*& window, i32 width, i32 height, const char* name) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, name, nullptr, nullptr);
  if (window == nullptr) {
    std::print("ERROR: Failed to create GLFW window\n");
    exit(1);
  }

  glfwMakeContextCurrent(window);

  int version = gladLoadGL(glfwGetProcAddress);
  if (!version) {
    std::print("ERROR: Failed to initialize GLAD\n");
    exit(1);
  }

  std::print("OpenGL version: {}.{}\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetKeyCallback(window, keyCallback);
  if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
    std::print("Using wayland\n");
  } else if (glfwGetPlatform() == GLFW_PLATFORM_X11) {
    std::print("Using X11\n");
  } else {
    std::print("Using other\n");
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 6.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}
