#include "window.h"


Window::Window(const WindowCreateInfo& window_info): 
    m_window(nullptr), m_window_width(window_info.height), m_window_height(window_info.width), m_success(true), m_window_should_close(false) {
    init(window_info);
}

Window::~Window() {
    SDL_DestroyWindow(m_window);
}

void Window::init(const WindowCreateInfo& window_info) {
    m_success = (SDL_Init(SDL_INIT_EVERYTHING) == 0);
    if (!m_success) {
        return;
    }
    m_window = SDL_CreateWindow(window_info.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_window_width, m_window_height, 0);
}

void Window::process_input() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            this->m_window_should_close = true;
            return;
       }    
    }
}
