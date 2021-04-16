#ifndef COMPUTE_SHADER_SRC_APP_APPLICATION_HPP_
#define COMPUTE_SHADER_SRC_APP_APPLICATION_HPP_

#include "app/ApplicationWindow.hpp"


class Application
{
public:
  Application();

  Application(const Application &other) = delete;
  Application(Application &&other) = delete;
  Application& operator=(const Application &other) = delete;
  Application& operator=(Application &&other) = delete;

  virtual ~Application() noexcept {};

public:
  void initializeGlfw(const uint32_t width, const uint32_t height);
  void initializeGlew();

  void loop();
  int run(int argc, char *argv[]);

private:
  ApplicationWindow window;
};

#endif /* COMPUTE_SHADER_SRC_APP_APPLICATION_HPP_ */
