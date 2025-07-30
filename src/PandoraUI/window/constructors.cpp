#include "../window.hpp"

using namespace PandoraUI;

Window::Window(PandoraEX::String title) : _title(title), _size(PandoraEX::Size2(800, 600)) {}

Window::Window(int width, int height, PandoraEX::String title) : _title(title), _size(PandoraEX::Size2(width, height)) {}

Window::Window(const Window &other) : _title(other._title.c_str()), _size(other._size), _isInitialized(other._isInitialized), _window(other._window), _fps(other._fps), _lastFrameTime(other._lastFrameTime)
{
    // Copy constructor
}

Window &Window::operator=(const Window &other)
{
    if (this != &other)
    {
        _title = other._title;
        _size = other._size;
        _isInitialized = other._isInitialized;
        _window = other._window;
        _fps = other._fps;
        _lastFrameTime = other._lastFrameTime;
    }
    return *this;
}

Window::~Window()
{
    // Destructor
}