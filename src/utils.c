#include "utils.h"

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

