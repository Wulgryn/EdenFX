#include "windowManager.hpp"
#include "IWindow.hpp"

#include "GLFW/glfw3.h"

#include <thread>

using namespace PandoraUI;

PandoraEX::AsyncList<IWindow> WindowManager::windows;
double WindowManager::currentFrameTime = 0;

const IWindow& WindowManager::registerWindow(const IWindow& window)
{
    windows.add(window);
    return windows.at(windows.indexOf(window));
}

void WindowManager::unregisterWindow(const IWindow& window)
{
    windows.remove(window);
}

void WindowManager::unregisterWindow(int index)
{
    windows.removeAt(index);
}

IWindow& WindowManager::getWindow(int index)
{
    return const_cast<IWindow&>(windows.at(index));
}

IWindow& WindowManager::getWindow(const IWindow& window)
{
    return const_cast<IWindow&>(windows.at(windows.indexOf(window)));
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
        // (windows[i]).close();
        ThrowExceptionF(PandoraEX::Exceptions::NotImplementedException, "Window::close() is not implemented yet.");
    }
}

void WindowManager::startUpdateLoop()
{
    while (getWindowCount() > 0)
    {
        for (int i = 0; i < getWindowCount(); i++)
        {
            getWindow(i)._update();
            // if(getWindow(i)._isClosing) WindowManager::unregisterWindow(getWindow(i));
            // ThrowExceptionF(PandoraEX::Exceptions::NotImplementedException, "Window::update() is not implemented yet.");
        }
        windows.validateRemovals();
        glfwPollEvents();
    }
}

int WindowManager::getWindowCount()
{
    return windows.size();
}

double WindowManager::getCurrentFrameTime()
{
    return currentFrameTime;
}