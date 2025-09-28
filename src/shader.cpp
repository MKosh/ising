#include "shader.h"

////////////////////////////////////////////////////////////////////////////////
///
const char* ReadFile(const char* filename)
{

  FILE* fptr = fopen(filename, "r");
  if (fptr == nullptr) {
    printf("ERROR: Cannot open shader file \"%s\" for reading\n", filename);
    return nullptr;
  }

  fseek(fptr, 0, SEEK_END);
  int len = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  char* source = (char*)malloc(len+1);

  if (fread(source, 1, len, fptr) == 0) {
    printf("ERROR: Shader file not read\n");
    return nullptr;
  }
  fclose(fptr);

  source[len] = 0;

  return source;
}

////////////////////////////////////////////////////////////////////////////////
///
u32 CompileShaders(ShaderInfo* shaders)
{
  if (shaders == nullptr) {
    printf("ERROR: No shaders to load/compile\n");
    return 0;
  }

  u32 program = glCreateProgram();
  int success = -1;
  char infoLog[512];

  int i = 0;
  while (shaders[i].type != GL_NONE) {
    shaders[i].shaderID = glCreateShader(shaders[i].type);
    const char* source = ReadFile(shaders[i].filename);
    if (source == nullptr) {
      return 0;
    }
    glShaderSource(shaders[i].shaderID, 1, &source, nullptr);
    glCompileShader(shaders[i].shaderID);

    glGetShaderiv(shaders[i].shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shaders[i].shaderID, 512, nullptr, infoLog);
      printf("ERROR: Compiling shader %s, of type %d\n%s\n", shaders[i].filename, shaders[i].type, infoLog);
    }

    glAttachShader(program, shaders[i].shaderID);
    free((char*)source);
    ++i;
  }

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    printf("ERROR: Linking shader program: %s\n", infoLog);
  }

  for (int iter = 0; iter <= i; ++iter) {
    glDeleteShader(shaders[iter].shaderID);
  }

  return program;
}

void set_uniform_float(u32 program, const char* uniform, float value)
{
  glUniform1f(glGetUniformLocation(program, uniform), value);
}

void set_uniform_int(u32 program, const char* uniform, int value)
{
  glUniform1i(glGetUniformLocation(program, uniform), value);
}

void set_uniform_bool(u32 program, const char* uniform, bool value)
{
  glUniform1i(glGetUniformLocation(program, uniform), (int)value);
}

// void set_uniform_mat4(u32 program, const char* uniform, glm::mat4 matrix)
// {
//   glUniformMatrix4fv(glGetUniformLocation(program, uniform), 1, GL_FALSE, matrix[0]);
// }

// void set_uniform_vec4(u32 program, const char* uniform, vec4 vec)
// {
//   glUniform4fv(glGetUniformLocation(program, uniform), 1, vec);
// }

