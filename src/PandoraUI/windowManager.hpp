#ifndef PANDORAUI_WINDOW_MANAGER_HPP
#define PANDORAUI_WINDOW_MANAGER_HPP

#include "PandoraEX/object.hpp"
#include "PandoraEX/asyncList.hpp"


namespace PandoraUI
{
    class Window;
    /// @brief Manages all windows in the application.
    /// @details This class is responsible for registering, unregistering, and managing the lifecycle of windows.
    Class(WindowManager)
    {
        static PandoraEX::List<Window> windows;
        static long currentFrameTime;
    public:
        /// @brief Registers a window with the WindowManager.
        /// @param window The window to register.
        static Window& registerWindow(const Window& window);

        /// @brief Unregisters a window from the WindowManager.
        /// @param window The window to unregister.
        static void unregisterWindow(const Window& window);

        /// @brief Unregisters a window by its index.
        /// @param index The index of the window to unregister.
        static void unregisterWindow(int index);

        /// @brief Gets a window by its index.
        /// @param index The index of the window.
        /// @return A reference to the window.
        static Window& getWindow(int index);

        /// @brief Starts the frame time update thread.
        static void startFrameTimeUpdateThread();

        /// @brief Closes all windows.
        static void closeAllWindows();

        /// @brief Starts the update loop for all registered windows.
        /// @note This method will continuously update all registered windows.
        static void startUpdateLoop();

        /// @brief Gets the number of registered windows.
        /// @return The number of registered windows.
        static int getWindowCount();

        /// @brief Gets the current frame time.
        /// @return The current frame time in milliseconds.
        static long getCurrentFrameTime();

    };
}

#endif // PANDORAUI_WINDOW_MANAGER_HPP