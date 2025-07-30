#include "windowManager.hpp"
#include "window.hpp"

#include "GLFW/glfw3.h"

#include <thread>

using namespace PandoraUI;

PandoraEX::List<Window> WindowManager::windows;
long WindowManager::currentFrameTime = 0;

void WindowManager::registerWindow(const Window& window)
{
    windows.add(window);
}

void WindowManager::unregisterWindow(const Window& window)
{
    windows.remove(window);
}

Window& WindowManager::getWindow(int index)
{
    return windows[index];
}

void WindowManager::startFrameTimeUpdateThread()
{
/* QUESTION: Is it detach or not?
 *?  *=======================================================================
 *?  * DESCRIPTION: Is it detach or just leave it?
 *?  *=======================================================================
 *?  */
    std::thread([]() {
        while (WindowManager::getWindowCount() > 0)
        {
            WindowManager::currentFrameTime = glfwGetTime();
        }
    }).detach();
}

void WindowManager::closeAllWindows()
{
    for (size_t i = 0; i < windows.size(); i++)
    {
        (windows[i]).close();
    }
}

int WindowManager::getWindowCount()
{
    return windows.size();
}

long WindowManager::getCurrentFrameTime()
{
    return currentFrameTime;
}