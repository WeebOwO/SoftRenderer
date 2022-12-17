#pragma once

#include <memory>

class Renderer;

class Application {
 public:
  void Run();
  static std::unique_ptr<Application> GetInstance(const char* title, int width, int height);

  Application() = default;
  Application(const Application& other) = delete;
  Application(Application&& other) noexcept = default;
  Application& operator=(const Application& other) = delete;
  ~Application();
  // other constructor
  Application(const char* title, int width, int height);

 private:
  void LogicTick();
  void RenderTick();

 private:
  std::unique_ptr<Renderer> m_renderer_{nullptr};
  bool m_running_{true};
};