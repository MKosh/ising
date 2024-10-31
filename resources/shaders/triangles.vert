#version 460 core

layout (location = 0) in vec3 vPosition;
// out vec4 frag_color;
uniform mat4 pvm;

void main() {
  gl_Position = pvm * vec4(vPosition, 1.0);
  // frag_color = color;
}
