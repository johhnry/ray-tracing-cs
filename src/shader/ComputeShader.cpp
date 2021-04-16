#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <string>
#include "log/Log.hpp"

#include "ComputeShader.hpp"


ComputeShader::ComputeShader(const char *filePath) :
  IShader(GL_COMPUTE_SHADER, filePath)
{
}

ComputeShader::~ComputeShader() noexcept {
  std::ostringstream address;
  address << this;
  Log::info("Destructor ComputeShader " + address.str());
}

void ComputeShader::printWorkGroupsCapabilities() {
  int workgroup_count[3];
  int workgroup_size[3];
  int workgroup_invocations;

  std::cout << std::endl;
  Log::info("----------------- COMPUTE SHADER INFO -----------------");

  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workgroup_count[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workgroup_count[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workgroup_count[2]);

  Log::info("Max size of workgroups: x(" +
               std::to_string(workgroup_size[0]) + "), y(" +
               std::to_string(workgroup_size[1]) + "), z(" +
               std::to_string(workgroup_size[2]) + ")");

  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workgroup_size[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workgroup_size[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workgroup_size[2]);

  Log::info("Max local invocation size: x(" +
               std::to_string(workgroup_size[0]) + "), y(" +
               std::to_string(workgroup_size[1]) + "), z(" +
               std::to_string(workgroup_size[2]) + ")");

  glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
  Log::info("Maximum number of invocations : " + std::to_string(workgroup_invocations));

  int max_shared_memory;

  glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &max_shared_memory);

  Log::info("Maximum shared memory available : " + std::to_string(max_shared_memory / 1000) + "KB");

  Log::info("-------------------------------------------------------");
  std::cout << std::endl;
}
