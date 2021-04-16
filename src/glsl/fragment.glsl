#version 460 core

in vec4 inColor;
in vec2 TexCoord;
out vec4 color;

uniform sampler2D currentTexture;

void main() {
  color = texture(currentTexture, 1.0 - TexCoord);
}
