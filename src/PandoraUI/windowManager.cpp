#include "windowManager.hpp"
#include "window.hpp"

#include "GLFW/glfw3.h"

#include <thread>

using namespace PandoraUI;

PandoraEX::List<Window> WindowManager::windows;
long WindowManager::currentFrameTime = 0;

Window& WindowManager::registerWindow(const Window& window)
{
    windows.add(window);
    return windows.at(windows.indexOf(window));
}

void WindowManager::unregisterWindow(const Window& window)
{
    windows.remove(window);
}

void WindowManager::unregisterWindow(int index)
{
    windows.removeAt(index);
}

Window& WindowManager::getWindow(int index)
{
    return windows.at(index);
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

void WindowManager::startUpdateLoop()
{
    while (getWindowCount() > 0)
    {
        for (int i = 0; i < getWindowCount(); i++)
        {
            getWindow(i).update();
        }
        // windows.validateRemovals();
        glfwPollEvents();
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