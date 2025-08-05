#include "../window.hpp"
#include "../../PandoraDebug/console.hpp"
#include "../windowManager.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

#include <thread>
#include <type_traits>

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
    this->fps = fps;
}

void Window::open()
{
    if (!this->_isInitialized)
    {
        DC::logFailure("Cannot open window that is not initialized.");
        return;
    }
    Window& w = WindowManager::registerWindow(*this);
    glfwSetWindowUserPointer(_window, &w);
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
    glClear(GL_COLOR_BUFFER_BIT);
    renderGUI();
    onRender.invoke(this);
    glfwSwapBuffers(_window);
}

void Window::update()
{
    if(_lastFrameTime + 1.0 / fps > WindowManager::getCurrentFrameTime()) return;
    _lastFrameTime = WindowManager::getCurrentFrameTime();
    render();
    onLateUpdate.invoke(this);
}