#include "../window.hpp"
#include "../../PandoraDebug/console.hpp"
#include "../windowManager.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

using namespace PandoraUI;
using namespace PandoraEX;
using namespace PandoraDebug;

void Window::initialize()
{
    if (this->_isInitialized) return;



    /* ADD: add later
     *+ *=================================ADD===================================
     *+ * DESCRIPTION: check if GPU acceleration is enabled...
     *+ *=======================================================================
     *+ */
    if (usecase == UsecaseType::GAME_WINDOW || usecase == UsecaseType::MIXED_WINDOW)
    {
    }

    _window = glfwCreateWindow(_size.width, _size.height, _title.c_str(), NULL, NULL);

    if (!_window)
    {
        glfwTerminate();
        DC::logFailure("Failed to create GLFW window");
        return;
    }
    DC::logSuccess("GLFW window created successfully");
    glfwMakeContextCurrent(_window);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        glfwTerminate();
        DC::logFailure("Failed to initialize GLAD");
        return;
    }
    DC::logSuccess("GLAD initialized successfully");
    
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow *window, int width, int height) {
        Window *win = (Window *)glfwGetWindowUserPointer(window);
        win->_size = Size2(width, height);
        glViewport(0, 0, width, height);
    });

    glfwSetWindowCloseCallback(_window, [](GLFWwindow *window) {
        Window *win = (Window *)glfwGetWindowUserPointer(window);
        WindowManager::unregisterWindow(*win);
        win->onLateUpdate = [](Window *win) {
            glfwDestroyWindow(win->_window);
            win->_window = NULL;
            win->_isInitialized = false;
            DC::logInfo("Window closed. \"%s\"", win->_title.c_str());
        };
    });


    this->_isInitialized = true;
}