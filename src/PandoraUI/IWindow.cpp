#include "IWindow.hpp"
#include "windowManager.hpp"
#include "PandoraDebug/console.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

using namespace PandoraUI;
using namespace PandoraUI;
using namespace PandoraDebug;

std::atomic<unsigned long long> IWindow::s_nextID{0};

IWindow::IWindow()
{
    _id = s_nextID++;
    title = PandoraEX::String("EdenFX Window[" + std::to_string(_id) + "]");
    title.onChange.add([this](const PandoraEX::String &newValue, const PandoraEX::String &oldValue)
                       {
        if (!_glfWindow)
        {
            DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
            DC::logWarning("Cannot change window title. GLFW window is null.");
            title.set(oldValue, false);
            return;
        }

        if(!_isInitialized)
        {
            DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
            DC::logWarning("Cannot change window title. Window is not initialized.");
            title.set(oldValue, false);
            return;
        }

        glfwSetWindowTitle(_glfWindow, newValue.c_str()); });
    size.width.onChange += [this](int newValue, int oldValue)
    {
        if (!_glfWindow)
        {
            DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
            DC::logWarning("Cannot change window size. GLFW window is null.");
            size.width.set(oldValue, false);
            return;
        }

        if (!_isInitialized)
        {
            DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
            DC::logWarning("Cannot change window size. Window is not initialized.");
            size.width.set(oldValue, false);
            return;
        }

        glfwSetWindowSize(_glfWindow, newValue, size.height);
    };
    size.height.onChange += [this](int newValue, int oldValue)
    {
        if (!_glfWindow)
        {
            DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
            DC::logWarning("Cannot change window size. GLFW window is null.");
            size.height.set(oldValue, false);
            return;
        }

        if (!_isInitialized)
        {
            DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
            DC::logWarning("Cannot change window size. Window is not initialized.");
            size.height.set(oldValue, false);
            return;
        }

        glfwSetWindowSize(_glfWindow, size.width, newValue);
    };

    fps.onChange += [this](int newValue, int oldValue)
    {
        if (newValue < -1)
        {

            fps.set(oldValue, false);
            DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
            DC::logWarning("FPS value cannot be less than 1. Setting to 1.");
        }
    };
}

void IWindow::initialize()
{
    if (_isInitialized)
        return;
    _glfWindow = glfwCreateWindow(size.width, size.height, title->c_str(), NULL, NULL);
    if (!_glfWindow)
    {
        glfwTerminate();
        DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
        DC::logFailure("Failed to create GLFW window");
        return;
    }
    DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
    DC::logSuccess("GLFW window created successfully");
    glfwMakeContextCurrent(_glfWindow);

    if (glad_glGetString == nullptr)
    {
        if (!gladLoadGL(glfwGetProcAddress))
        {
            glfwTerminate();
            DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
            DC::logFailure("Failed to initialize GLAD");
            return;
        }
        DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
        DC::logSuccess("GLAD initialized successfully");
    }
    else
    {
        DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
        DC::logInfo("GLAD already initialized");
    }

    glfwSetWindowUserPointer(_glfWindow, this);
    glfwSetFramebufferSizeCallback(_glfWindow, [](GLFWwindow *window, int width, int height)
                                   {
        IWindow *win = (IWindow *)glfwGetWindowUserPointer(window);
        win->size.set(width, height,false); });

    glfwSetWindowCloseCallback(_glfWindow, [](GLFWwindow *glfwwindow)
                               {
        IWindow *win = (IWindow *)glfwGetWindowUserPointer(glfwwindow);
        win->_close(); });
    WindowManager::registerWindow(*this);
    _isInitialized = true;
}

void IWindow::_update()
{
    if (fps > 0 && _lastFrameTime + 1.0 / fps > WindowManager::getCurrentFrameTime())
        return;
    _lastFrameTime = WindowManager::getCurrentFrameTime();
    onUpdate.invoke(*this);
    _render();
    onLateUpdate.invoke(*this);
}

void IWindow::_render()
{
    glfwMakeContextCurrent(_glfWindow);
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, size.width, size.height);
    onRender.invoke(*this);
    render();
    onLateRender.invoke(*this);
    glfwSwapBuffers(_glfWindow);
}

void IWindow::_close()
{
    WindowManager::unregisterWindow(*this);
    WindowManager::getWindow(*this).onLateUpdate = [](IWindow &win)
    {
        glfwDestroyWindow(win._glfWindow);
        win._glfWindow = NULL;
        DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", win._id);
        DC::logInfo("Window closed -> \"%s\"", win.title->c_str());
    };
}

void IWindow::show()
{
    if (!_glfWindow)
    {
        DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
        DC::logFailure("Cannot open window. GLFW window is null.");
        return;
    }

    if (!_isInitialized)
    {
        DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
        DC::logFailure("Cannot open window. Window is not initialized.");
        return;
    }
    glfwShowWindow(_glfWindow);
    DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
    DC::logInfo("Window Showed. \"%s\"", title->c_str());
}

void IWindow::close()
{
    if (!_glfWindow)
    {
        DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
        DC::logWarning("Cannot close window. GLFW window is already null.");
        return;
    }
    if (!_isInitialized)
    {
        DC::contextInfoOnce = Utils::format("<IWindow[%llu]>", this->_id);
        DC::logWarning("Cannot close window. Window is not initialized.");
        return;
    }

    _close();
}