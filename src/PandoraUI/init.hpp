#ifndef PANDORAUI_INIT_HPP
#define PANDORAUI_INIT_HPP

#include "../PandoraEX/string.hpp"

namespace PandoraUI
{
    /// @brief Enumeration for initialization status codes.
    enum class InitStatusCode
    {
        /// @brief Initialization was successful.
        Success,
        /// @brief Initialization failed.
        Failure,
        /// @brief Initialization was already done.
        AlreadyInitialized,
        /// @brief Initialization was not done yet.
        NotInitialized
    };

    /// @brief Class representing the status of the initialization process.
    class InitStatus
    {
        const InitStatusCode code;
        const PandoraEX::String message;
        static bool initialized;
    public:
        /// @brief Constructs an InitStatus with a specific status code and an optional message.
        /// @param statusCode The status code indicating the result of the initialization.
        /// @param msg An optional message providing additional information about the status.
        /// If no message is provided, it defaults to an empty string.
        /// @note The message can be used to provide context or details about the initialization status.
        InitStatus(InitStatusCode statusCode, PandoraEX::String msg = "");

        /// @brief Checks if the initialization was successful.
        /// @return True if the initialization was successful, false otherwise.
        bool isSuccess() const;
        
        /// @brief Checks if the initialization failed.
        /// @return True if the initialization failed, false if it was successful.
        bool isFailure() const;

        /// @brief Checks if the initialization has already been done.
        /// @return True if the initialization has already been done, false otherwise.
        static bool isInitialized();

        /// @brief Gets the status code of the initialization.
        /// @return The status code indicating the result of the initialization.
        InitStatusCode getCode() const;

        /// @brief Gets the message associated with the initialization status.
        /// @return A reference to the message string providing additional context about the initialization status.
        /// @note The message can be empty if no message was provided during construction.
        const PandoraEX::String &getMessage() const;
    };

    /// @brief Initializes the PandoraUI library.
    /// @return An InitStatus object indicating the result of the initialization.
    /// @note This function should be called before using any other functions in the PandoraUI library.
    InitStatus initialize();

    /// @brief Starts the PandoraUI event loop.
    /// @return The exit code of the event loop.
    /// @note This function blocks until the event loop is exited, typically when the user closes the application.
    int waitForExit();
}

#endif // PANDORAUI_INIT_HPPs