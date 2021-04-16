#ifndef COMPUTE_SHADER_SRC_SHADER_COMPUTEPROGRAM_HPP_
#define COMPUTE_SHADER_SRC_SHADER_COMPUTEPROGRAM_HPP_

#include "shader/Program.hpp"
#include <bits/stdint-uintn.h>


class ComputeProgram: public Program
{
public:
  ComputeProgram(uint32_t workX, uint32_t workY, uint32_t workZ);

  ComputeProgram(const ComputeProgram &other) = delete;
  ComputeProgram(ComputeProgram &&other) = delete;
  ComputeProgram& operator=(const ComputeProgram &other) = delete;
  ComputeProgram& operator=(ComputeProgram &&other) = delete;

  virtual ~ComputeProgram() noexcept {};

public:
  void dispatch() const;
  void memoryBarrier() const;

private:
  uint32_t workX, workY, workZ;
};

#endif /* COMPUTE_SHADER_SRC_SHADER_COMPUTEPROGRAM_HPP_ */


