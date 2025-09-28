#pragma once

#include "glad/gl.h"
#include "utils.h"
#include "stdio.h"
#include "stdlib.h"

#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////
///
struct ShaderInfo {
  GLenum      type;
  const char* filename;
  u32         shaderID;
} ShaderInfo;

const char* ReadFile(const char* filename);
u32 CompileShaders(ShaderInfo* shaders);
void set_uniform_float(u32 program, const char* uniform, float value);
void set_uniform_int(u32 program, const char* uniform, int value);
void set_uniform_bool(u32 program, const char* uniform, bool value);
