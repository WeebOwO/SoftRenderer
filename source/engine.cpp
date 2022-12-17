
#include "engine.h"

#include <SDL2/SDL.h>

#include "render_system.h"
#include "runtime_context.h"

void Application::Run() {
  while (m_running_) {
    LogicTick();
    RenderTick();
  }
}

void Application::LogicTick() {
  static SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        m_running_ = false;
        break;
    }
  }
}

void Application::RenderTick() {
  auto &&context = RuntimeContext::GetInstance();
  m_renderer_->RenderWithContext(context);
}

Application::~Application() { SDL_Quit(); }

Application::Application(const char *title, int width, int height) {
  WindowInfo info;
  info.title_ = title;
  info.width_ = width;
  info.height_ = height;
  m_renderer_ = std::make_unique<Renderer>(info);
}

std::unique_ptr<Application> Application::GetInstance(const char *title, int width, int height) {
  static auto instance = std::make_unique<Application>(title, width, height);
  return std::move(instance);
}
