#version 460 core

layout (location = 0) out vec4 fColor;
// in vec4 frag_color;
uniform vec4 frag_color;

void main() {
  // fColor = vec4(0.1, 0.1, 0.1, 1.0);
  fColor = frag_color;
}
