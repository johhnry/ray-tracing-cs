#include <bits/stdint-uintn.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <unistd.h>

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "utils/FileUtils.hpp"
#include "log/Log.hpp"

#include "shader/ComputeProgram.hpp"
#include "shader/ComputeShader.hpp"
#include "shader/FragmentShader.hpp"
#include "shader/Program.hpp"
#include "shader/VertexShader.hpp"

#include "geometry/FullScreenVAO.hpp"
#include "texture/Texture.hpp"

#include "Application.hpp"


Application::Application(): window() {}

void Application::initializeGlfw(const uint32_t width, const uint32_t height)
{
  int glfw = glfwInit();

  if (glfw != GLFW_TRUE) {
    throw "GLFW: Couldn't initialize correctly";
  }

  Log::success("GLFW: Successfully initialized");

  // Setting glfw callbacks for errors
  glfwSetErrorCallback(Log::errorCallBack);

  Log::message("GLFW: Running version " + std::string(glfwGetVersionString()));

  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Initialize window
  window.initialize("Compute shaders : GPU ray tracing", width, height);

  // Set frame interval swap
  glfwSwapInterval(1);

  // Enable depth
  glEnable(GL_DEPTH_TEST);

  // Enable multi sampling
  glfwWindowHint(GLFW_SAMPLES, 4);
  glEnable(GL_MULTISAMPLE);
}

void Application::initializeGlew()
{
  GLenum err = glewInit();

  if (err != GLEW_OK) {
    std::string glfwError = std::string(reinterpret_cast<const char *>(glewGetErrorString(err)));
    throw "GLEW initialization error : " + glfwError;
  }

  std::string glewVersion = std::string(reinterpret_cast<const char *>(glewGetString(GLEW_VERSION)));
  Log::success("Using GLEW: " + glewVersion);
}

int Application::run(int argc, char **argv)
{
  // Print arguments
  std::string args = "";
  for (int i = 0; i < argc; i++) args += std::string(argv[i]) + " ";
  Log::info("Command parameters (" + std::to_string(argc) + ") : " + args);
  std::cout << std::endl;

  uint32_t windowWidth = 1024;
  uint32_t windowHeight = 512;
  int scene = 1;

  // Get arguments
  if (argc >= 2) {
    std::istringstream sceneStream(argv[1]);
    int sceneInput;
    if (sceneStream >> sceneInput) scene = sceneInput;
  }

  if (argc >= 4) {
    std::istringstream widthStream(argv[2]);
    std::istringstream heightStream(argv[3]);
    uint32_t widthInput, heightInput;

    if (widthStream >> widthInput && heightStream >> heightInput) {
      windowWidth = widthInput;
      windowHeight = heightInput;
    }
  }

  // Initialize both GLEW and GLFW
  try {
    initializeGlfw(windowWidth, windowHeight);
    initializeGlew();
  } catch (const std::string &error) {
    Log::error(error);
    return EXIT_FAILURE;
  }

  // Create texture
  Texture texture2D(window.getWidth(), window.getHeight());

  // Create VAO
  FullScreenVAO fullScreenVAO;

  // Create compute shader
  ComputeShader::printWorkGroupsCapabilities();
  ComputeProgram computeProgram(window.getWidth(), window.getHeight(), 1);
  Program displayProgram;

  // Compile different shaders
  try {
    // Compute shader
    std::string csPath = "./src/glsl/compute" + std::to_string(scene) + ".glsl";
    ComputeShader computeShader(csPath.c_str());
    computeShader.compile();

    computeProgram.attachShader(computeShader);
    computeProgram.link();

    // Vertex shader and fragment shader
    VertexShader vertexShader("./src/glsl/vertex.glsl");
    FragmentShader fragmentShader("./src/glsl/fragment.glsl");

    vertexShader.compile();
    fragmentShader.compile();

    displayProgram.attachShader(vertexShader);
    displayProgram.attachShader(fragmentShader);

    displayProgram.link();
  } catch (const std::string &error) {
    Log::error(error);
    return EXIT_FAILURE;
  }

  // Game loop
  while (!glfwWindowShouldClose(window.getGLFWwindow())) {
    // Get time
    float timeValue = glfwGetTime() / 2.0;

    // COMPUTE SHADER EXECUTION --------------------------
    computeProgram.beginUse();

    // Set uniforms
    computeProgram.setFloat("time", timeValue);

    switch (scene) {
    case 1:
      computeProgram.setVec3("cameraPos", std::cos(timeValue) * 5.0,
                             (std::cos(timeValue / 2.0) + 1) * 2,
                             std::sin(timeValue) * 5.0);
      computeProgram.setVec3("cameraLookAt", std::sin(timeValue / 10.0), 0.0,
                             std::cos(timeValue / 10.0));
      break;
    case 2:
      computeProgram.setVec3("cameraPos", std::cos(timeValue) * 5.0,
                             (std::sin(timeValue / 2.0) + 1) * 2,
                             std::cos(timeValue) * 5.0);
      computeProgram.setVec3("cameraLookAt", std::sin(timeValue / 10.0), 0.0,
                             std::cos(timeValue / 10.0));
      break;
    default:
      break;
    }

    // Bind texture and dispatch
    texture2D.bind();
    texture2D.bindImageTexture();

    computeProgram.dispatch();
    computeProgram.memoryBarrier();

    texture2D.unbindImageTexture();
    texture2D.unbind();

    computeProgram.endUse();
    // COMPUTE SHADER EXECUTION --------------------------

    // GLFW events and clear window
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // DISPLAY TEXTURE -----------------------------------
    displayProgram.beginUse();
    fullScreenVAO.bind();
    texture2D.beginUse();

    fullScreenVAO.draw();

    texture2D.endUse();
    fullScreenVAO.unbind();
    displayProgram.endUse();
    // DISPLAY TEXTURE -----------------------------------

    // Swap buffers
    glfwSwapBuffers(window.getGLFWwindow());
  }

  // Exit
  Log::message("Exiting program...");
  std::cout << std::endl;
  return EXIT_SUCCESS;
}
