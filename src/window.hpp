#pragma once

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "utils.h"

struct Window {
  i32 width = 1280;
  i32 height= 720;
  GLFWwindow* win = nullptr;
};

void initWindow(GLFWwindow*& window, i32 width, i32 height, const char* name);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
