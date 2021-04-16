#include <string>

#include "log/Log.hpp"

#include "ComputeProgram.hpp"

ComputeProgram::ComputeProgram(uint32_t workX, uint32_t workY, uint32_t workZ):
  workX(workX), workY(workY), workZ(workZ)
{
}

void ComputeProgram::dispatch() const
{
  glDispatchCompute(workX, workY, workZ);
}

void ComputeProgram::memoryBarrier() const
{
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
