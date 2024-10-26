#include <stdlib.h>
#include <stdio.h>

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#define WINWIDTH 800
#define WINHEIGHT 600

int main(int argc, char *argv[])
{
  (void)printf("hello world\n");

  if (!glfwInit()) {
    (void)printf("ERROR: initializing glfw\n");
    return EXIT_FAILURE;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WINWIDTH, WINHEIGHT, "2D Ising Model", nullptr, nullptr);
  if (window == nullptr) {
    (void)printf("ERROR: can't open window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  int glad_version = gladLoadGL(glfwGetProcAddress);
  if (glad_version == 0) {
    (void)printf("ERROR: Couldn't load GLAD\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  (void)printf("Loaded OpenGL: %d.%d\n", GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));

  glViewport(0, 0, WINWIDTH, WINHEIGHT);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
