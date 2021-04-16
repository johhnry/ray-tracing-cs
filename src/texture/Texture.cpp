#include "Texture.hpp"


Texture::Texture(uint32_t width, uint32_t height)
{
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
}

GLuint Texture::getId() const
{
  return id;
}

void Texture::beginUse() const
{
  glActiveTexture(GL_TEXTURE0);
  bind();
}

void Texture::endUse() const
{
  glActiveTexture(0);
  unbind();
}

void Texture::bind() const
{
  glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindImageTexture() const
{
  glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                     GL_RGBA32F);
}

void Texture::unbindImageTexture() const
{
  glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}
