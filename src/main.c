#include "main.h"
#include "GLFW/glfw3.h"

#include "cglm/mat4.h"
#include "utils.h"
#include "shader.h"

#define CGLM_OMIT_NS_FROM_STRUCT_API
#include <cglm/cglm.h>
#include <cglm/struct.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define WINWIDTH 800
#define WINHEIGHT 600
#define GRIDSIZE 16

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

typedef struct Quad {
  u32 VAO;
  u32 VBO;
  u32 EBO;
  u32 shader_program;
} Quad;

Quad make_quad()
{
  Quad q;
  float vertices[] = {
     0.5f,  0.5f, -10.0f,
     0.5f, -0.5f, -10.0f,
    -0.5f, -0.5f, -10.0f,
    -0.5f,  0.5f, -10.0f,
  };
  u32 indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  glCreateVertexArrays(1, &q.VAO);
  glCreateBuffers(1, &q.VBO);
  glCreateBuffers(1, &q.EBO);

  glBindVertexArray(q.VAO);

  glNamedBufferStorage(q.VBO, sizeof(vertices), vertices, 0);
  glBindBuffer(GL_ARRAY_BUFFER, q.VBO);
  glNamedBufferStorage(q.EBO, sizeof(indices), indices, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, q.EBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), BUFFER_OFFSET(0));
  glEnableVertexAttribArray(0);

  return q;
}

u32 init_grid(GLFWwindow* window, u32 size, u32* VAOout)
{
  i32 win_width = 0;
  i32 win_height = 0;
  glfwGetWindowSize(window, &win_width, &win_height);

  /* float* vertices = (float*)malloc(9*sizeof(float)); */
  /* memcpy(vertices, (float []){ */
  /*   -0.5f, -0.5f, 0.0f, */
  /*    0.5f, -0.5f, 0.0f, */
  /*    0.0f,  0.5f, 0.0f */
  /* }, 9); */

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  u32 VBO, EBO, VAO;
  glCreateBuffers(1, &VBO);
  glNamedBufferStorage(VBO, sizeof(vertices), vertices, 0);

  ShaderInfo shaders[] = {
    { .type = GL_VERTEX_SHADER,   .filename = "resources/shaders/triangles.vert"},
    { .type = GL_FRAGMENT_SHADER, .filename = "resources/shaders/triangles.frag"},
    { .type = GL_NONE, nullptr}
  };

  u32 program = CompileShaders(shaders);

  glCreateVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), BUFFER_OFFSET(0));
  glEnableVertexAttribArray(0);
  *VAOout = VAO;
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

  // These calls are needed because at least on my system (NixOS, glfw3.4, Wayland)
  // framebuffer only renders in the bottom left quadrant until the window is resized.
  // Resizing before even rendering the window avoids this issue.
  glfwSetWindowSize(window, WINWIDTH, WINHEIGHT-1);
  glfwSetWindowSize(window, WINWIDTH, WINHEIGHT);

  Quad quad = make_quad();
  glBindVertexArray(quad.VAO);

  ShaderInfo shaders[] = {
    { .type = GL_VERTEX_SHADER,   .filename = "resources/shaders/triangles.vert"},
    { .type = GL_FRAGMENT_SHADER, .filename = "resources/shaders/triangles.frag"},
    { .type = GL_NONE, nullptr}
  };


  quad.shader_program = CompileShaders(shaders);
  glUseProgram(quad.shader_program);

  mat4 m;
  mat4 p;
  glm_mat4_identity(p);
  glm_mat4_identity(m);
  /* glm_translate(m, (vec3){0.5f, 0.0f, 0.0f}); */

  mat4 pvm;
  glm_perspective(glm_rad(45.0f), (float)WINWIDTH/WINHEIGHT, 0.1f, 100.0f, p);
  set_uniform_mat4(quad.shader_program, "pvm", pvm);
  set_uniform_vec4(quad.shader_program, "frag_color", (vec4){0.6f, 0.2f, 0.4f, 1.0f});

  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

  while (!glfwWindowShouldClose(window)) {
    glClearBufferfv(GL_COLOR, 0, clear_color);
    /* glm_rotate(m, glm_rad(1), (float[]){0.0f, 0.0f, 1.0f}); */
    glm_mat4_identity(m);
    glm_rotate(m, glfwGetTime()*glm_rad(50.0f), (float[]){0.0f, 0.0f, 1.0f});
    glm_mul(p, m, pvm);
    set_uniform_mat4(quad.shader_program, "pvm", pvm);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}
