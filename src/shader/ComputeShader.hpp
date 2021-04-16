#ifndef COMPUTE_SHADER_SRC_SHADER_COMPUTESHADER_HPP_
#define COMPUTE_SHADER_SRC_SHADER_COMPUTESHADER_HPP_

#include "GL/glew.h"

#include "shader/IShader.hpp"


class ComputeShader: public IShader
{
public:
  ComputeShader(const char *filePath);

  ComputeShader(const ComputeShader &other) = delete;
  ComputeShader(ComputeShader &&other) = delete;
  ComputeShader& operator=(const ComputeShader &other) = delete;
  ComputeShader& operator=(ComputeShader &&other) = delete;

  virtual ~ComputeShader() noexcept;

  // Static
  static void printWorkGroupsCapabilities();
};

#endif /* COMPUTE_SHADER_SRC_SHADER_COMPUTESHADER_HPP_ */

