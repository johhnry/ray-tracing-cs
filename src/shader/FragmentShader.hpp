#ifndef COMPUTE_SHADER_SRC_SHADER_FRAGMENTSHADER_HPP_
#define COMPUTE_SHADER_SRC_SHADER_FRAGMENTSHADER_HPP_

#include "GL/glew.h"

#include "shader/IShader.hpp"


class FragmentShader: public IShader
{
public:
  FragmentShader(const char *filePath);

  FragmentShader(const FragmentShader &other) = delete;
  FragmentShader(FragmentShader &&other) = delete;
  FragmentShader& operator=(const FragmentShader &other) = delete;
  FragmentShader& operator=(FragmentShader &&other) = delete;

  virtual ~FragmentShader() noexcept;
};

#endif /* COMPUTE_SHADER_SRC_SHADER_FRAGMENTSHADER_HPP_ */


