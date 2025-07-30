#include "init.hpp"
#include "windowManager.hpp"
#include "window.hpp"
#include "../PandoraDebug/console.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

#include <conio.h>

using namespace PandoraUI;
// using namespace PandoraDebug;
using namespace PandoraEX;
using namespace std;

typedef PandoraDebug::DC DC;

bool InitStatus::initialized = false;
int PandoraUI::exitCode = 0;

InitStatus::InitStatus(InitStatusCode statusCode, PandoraEX::String msg)
    : code(statusCode), message(std::move(msg))
{
    if (statusCode == InitStatusCode::Success)
        initialized = true;
}

bool InitStatus::isSuccess() const
{
    return code == InitStatusCode::Success;
}

bool InitStatus::isFailure() const
{
    return code != InitStatusCode::Success;
}

bool InitStatus::isInitialized()
{
    return initialized;
}

InitStatusCode InitStatus::getCode() const
{
    return code;
}

const PandoraEX::String &InitStatus::getMessage() const
{
    return message;
}

void __glfw_error_callback(int error, const char *description)
{
    DC::logError("[GLFW Error/" + to_string(error) + "] " + PandoraEX::String(description));
}

InitStatus PandoraUI::initialize()
{
    if (InitStatus::isInitialized())
    {
        DC::logWarning(" PandoraUI is already initialized. Skipping re-initialization.");
        return InitStatus(InitStatusCode::AlreadyInitialized, "PandoraUI is already initialized.");
    }

    glfwSetErrorCallback(__glfw_error_callback);
    DC::logInfo(" GLFW error callback set.");

    if (!glfwInit())
    {
        DC::logError(" Failed to initialize GLFW!");
        return InitStatus(InitStatusCode::Failure, "Failed to initialize GLFW.");
    }
    DC::logSuccess(" GLFW initialized successfully!");

    // glfwMakeContextCurrent(glfwCreateWindow(800, 600, "PandoraUI Window", nullptr, nullptr));
    // if (!gladLoadGL(glfwGetProcAddress))
    // {
    //     DC::logError(" Failed to initialize GLAD!");
    //     glfwTerminate();
    //     return InitStatus(InitStatusCode::Failure, "Failed to initialize GLAD.");
    // }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    DC::logInfo(" GLFW context version set: 4.6");
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    DC::logInfo(" GLFW OpenGL profile set: CORE");
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    DC::logInfo(" GLFW OpenGL forward compatibility set: TRUE");

    DC::logInfo(" GLFW window hints set.");
    DC::logInfo(" GLFW initialization complete.");
    DC::logInfo(" GLFW version: " + PandoraEX::String(glfwGetVersionString()));
    DC::logSuccess(" PandoraUI initialized successfully!");

    return InitStatus(InitStatusCode::Success, "PandoraUI initialized successfully.");
}

int PandoraUI::waitForExit(bool pauseOnExit)
{
    DC::logInfo(" Starting PandoraUI event loop...");

    WindowManager::startFrameTimeUpdateThread();
    while (WindowManager::getWindowCount() > 0)
    {
        for (int i = 0; i < WindowManager::getWindowCount(); i++)
        {
            WindowManager::getWindow(i).update();
        }
        glfwPollEvents();
    }
    glfwTerminate();

    DC::logInfo(" Exiting PandoraUI event loop.");
    if(!DC::isAttached()) DC::attach();
    if (pauseOnExit)
    {
        DC::logInfo(" Exiting with code: " + to_string(exitCode));
        if (exitCode == 0)
            DC::logSuccess(" Exiting successfully.");
        else
            DC::logError(" Exiting with an error code: " + to_string(exitCode));
        DC::logInfo(" Press any key to continue...");
        _getch();
    }
    return exitCode;
}