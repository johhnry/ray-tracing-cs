#ifndef COMPUTE_SHADER_SRC_TEXTURE_TEXTURE_HPP_
#define COMPUTE_SHADER_SRC_TEXTURE_TEXTURE_HPP_

#include "GL/glew.h"

class Texture
{
public:
  Texture(uint32_t width, uint32_t height);

  Texture(const Texture &other) = delete;
  Texture(Texture &&other) noexcept = delete;
  Texture& operator=(const Texture &other) = delete;
  Texture& operator=(Texture &&other) = delete;

  virtual ~Texture() noexcept {};

public:
  GLuint getId() const;

  void beginUse() const;
  void endUse() const;

  void bind() const;
  void unbind() const;

  void bindImageTexture() const;
  void unbindImageTexture() const;

private:
  GLuint id;
};

#endif /* COMPUTE_SHADER_SRC_TEXTURE_TEXTURE_HPP_ */
