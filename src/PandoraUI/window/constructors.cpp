#include "../window.hpp"
#include "../../PandoraDebug/console.hpp"

using namespace PandoraDebug;
using namespace PandoraEX;
using namespace PandoraUI;

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

void Window::constructClass()
{
    backgroundColor.onChange += [](PandoraEX::Color newColor, PandoraEX::Color)
    {
        glClearColor(newColor.r, newColor.g, newColor.b, newColor.a);
    };

    title.onChange += [this](PandoraEX::String newTitle, PandoraEX::String)
    {
        glfwSetWindowTitle(_window, newTitle.c_str());
    };

    size.onChange += [this](PandoraEX::Size2 newSize, PandoraEX::Size2)
    {
        if (_window)
        {
            glfwSetWindowSize(_window, newSize.width, newSize.height);
            glViewport(0, 0, newSize.width, newSize.height);
        }
    };

    fps.onChange += [this](int newFps, int oldFps)
    {
        if (newFps < -1)
        {
            DC::logWarning("FPS cannot be less than -1.");
            this->fps = oldFps; // Revert to old FPS
            return;
        }
    };

    // if(_isInitialized)glfwSetWindowUserPointer(_window, this);
}

Window::Window(PandoraEX::String title) : title(title)
{
    PDC::logTrace("Window Contructor(id): %d", id());
    constructClass();
}

Window::Window(int width, int height, PandoraEX::String title) : title(title), size(PandoraEX::Size2(width, height))
{
    PDC::logTrace("Window Contructor(id): %d", id());
    constructClass();
}

Window::Window(const Window &other) : Object(other), _window(other._window)
{
    PDC::logTrace("Window Copy Constructor(id): %d", id());
    title = other.title;
    size = other.size;
    _isInitialized = other._isInitialized;
    fps = other.fps;
    _lastFrameTime = other._lastFrameTime;

    // DC::logWarning("Copy constructor of window is not implemented yet.");
    constructClass();
}

Window &Window::operator=(const Window &other)
{
    PDC::logTrace("Window Assignment Operator(id): %d", id());
    if (this != &other)
    {
        title = other.title;
        size = other.size;
        _isInitialized = other._isInitialized;
        _window = other._window;
        fps = other.fps;
        _lastFrameTime = other._lastFrameTime;
    }
    return *this;
}

Window::~Window()
{
    PDC::logTrace("Window Destructor(id): %d", id());
    // Destructor
}