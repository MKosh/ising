#include "main.h"
#include "GLFW/glfw3.h"

#include "utils.h"
#include "shader.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define WINWIDTH 800
#define WINHEIGHT 600

static const float clear_color[] = { 0.3f, 0.5f, 0.8f, 1.0f};

enum VAO_IDs {
  Triangles,
  NumVAOs
};

enum Buffer_IDs {
  ArrayBuffer,
  NumBuffers
};

enum Attrib_IDs {
  vPosition = 0
};

u32 VAOs[NumVAOs];
u32 Buffers[NumBuffers];

const u32 NumVertices = 6;

u32 init()
{
   static const float vertices[6][2] =
    {
      { -0.90f, -0.90f },
      {  0.85f, -0.90f },
      { -0.90f,  0.85f },
      {  0.90f, -0.85f },
      {  0.90f,  0.90f },
      { -0.85f,  0.90f }
    };

  glCreateBuffers(NumBuffers, Buffers);
  glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);

  ShaderInfo shaders[] = {
    { .type = GL_VERTEX_SHADER,   .filename = "resources/shaders/triangles.vert"},
    { .type = GL_FRAGMENT_SHADER, .filename = "resources/shaders/triangles.frag"},
    { .type = GL_NONE, nullptr}
  };

  u32 program = CompileShaders(shaders);
  glUseProgram(program);

  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);

  return program;
}

int main(int argc, char *argv[])
{
  assert(sizeof(u32) == sizeof(GLuint));

  if (!glfwInit()) {
    printf("ERROR: initializing glfw\n");
    return EXIT_FAILURE;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  /* if (glfwPlatformSupported(GLFW_PLATFORM_X11)) { */

  GLFWwindow* window = glfwCreateWindow(WINWIDTH, WINHEIGHT, "2D Ising Model", nullptr, nullptr);
  if (window == nullptr) {
    printf("ERROR: can't open window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  int glad_version = gladLoadGL(glfwGetProcAddress);
  if (glad_version == 0) {
    printf("ERROR: Couldn't load GLAD\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  printf("Loaded OpenGL: %d.%d\n", GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));

  glViewport(0, 0, WINWIDTH, WINHEIGHT);

  auto prog = init();
  glUseProgram(prog);

  while (!glfwWindowShouldClose(window)) {
    glClearBufferfv(GL_COLOR, 0, clear_color);

    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
