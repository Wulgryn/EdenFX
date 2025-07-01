#include "init.hpp"

using namespace PandoraUI;

bool InitStatus::initialized = false;

InitStatus::InitStatus(InitStatusCode statusCode, PandoraEX::String msg)
    : code(statusCode), message(std::move(msg))
{
    if (statusCode == InitStatusCode::Success) initialized = true;
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

InitStatus PandoraUI::initialize()
{
    if (InitStatus::isInitialized())
    {
        return InitStatus(InitStatusCode::AlreadyInitialized, "PandoraUI is already initialized.");
    }

    // Perform initialization logic here
    // ...

    // If initialization is successful
    return InitStatus(InitStatusCode::Success, "PandoraUI initialized successfully.");
}