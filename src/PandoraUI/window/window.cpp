#include "../window.hpp"
#include "../../PandoraDebug/console.hpp"
#include "../windowManager.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

#include <thread>

using namespace PandoraUI;
using namespace PandoraEX;
using namespace PandoraDebug;

void Window::setFPS(int fps)
{
    if (fps < -1)
    {
        DC::logWarning("FPS cannot be less than -1.");
        return;
    }
    this->_fps = fps;
}

void Window::open()
{
    if (!this->_isInitialized)
    {
        DC::logFailure("Cannot open window that is not initialized.");
        return;
    }
    WindowManager::registerWindow(*this);
}

void Window::close()
{
    glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

void Window::waitForExit()
{
    if (this->_thread != NULL)
    {
        this->_thread->join();
    }
}

void Window::render()
{
    glfwMakeContextCurrent(_window);   
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
    renderGUI();
    onRender.Invoke(this);
    glfwSwapBuffers(_window);
}

void Window::update()
{
    if(_lastFrameTime + 1.0 / _fps > WindowManager::getCurrentFrameTime()) return;
    _lastFrameTime = WindowManager::getCurrentFrameTime();
    render();
    onLateUpdate.Invoke(this);
}