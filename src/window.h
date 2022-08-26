#pragma once
#include <string>

#include <SDL.h>

struct WindowCreateInfo {
    int width {1280};
    int height {720};
    const char* title{"CoreEngine"};
};

class Window {
private:
    SDL_Window* m_window;
    int m_window_width;
    int m_window_height;
    bool m_success, m_window_should_close;
public:

    Window() = delete;
    Window(const WindowCreateInfo& window_info);

    ~Window();

    void init(const WindowCreateInfo&);
    void process_input();

    inline bool window_should_close() { return m_window_should_close; }
    inline SDL_Window* get_window() const { return m_window; }
    inline int get_window_height() const { return m_window_height; }
    inline int get_window_width() const { return m_window_width; }
    inline bool check() const { return m_success; }
};

