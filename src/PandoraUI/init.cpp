#include "init.hpp"
#include "../PandoraDebug/console.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

using namespace PandoraUI;
using namespace PandoraDebug;
using namespace PandoraEX;
using namespace std;

bool InitStatus::initialized = false;

InitStatus::InitStatus(InitStatusCode statusCode, String msg)
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

const String &InitStatus::getMessage() const
{
    return message;
}

void __glfw_error_callback(int error, const char *description)
{
    DC::logError("[GLFW Error/" + to_string(error) + "] " + String(description));
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
    DC::logInfo(" GLFW version: " + String(glfwGetVersionString()));
    DC::logSuccess(" PandoraUI initialized successfully!");

    return InitStatus(InitStatusCode::Success, "PandoraUI initialized successfully.");
}