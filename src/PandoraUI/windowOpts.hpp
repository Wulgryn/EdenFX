#ifndef PANDORAUI_WINDOW_OPTIONS_HPP
#define PANDORAUI_WINDOW_OPTIONS_HPP

namespace PandoraUI
{
    /// @brief The type of GPU API to use for rendering.
    /// @note This is used to determine which GPU API will be used for rendering.
    enum class GPU_API
    {
        OPENGL,
        VULKAN,
        DIRECTX11,
        DIRECTX12
    };

    /// @brief The type of usecase for the window.
    /// @note This is used to determine how the window will be used and what features it will support.
    enum class UsecaseType
    {
        /// @brief A window for a game.
        /// @note Uses the Scenes.
        GAME_WINDOW,
        /// @brief A window for a GUI.
        /// @note Does not depend on the Scenes. Only UI elements.
        GUI_WINDOW,
        /// @brief A window for a mixed use of GUI and Game.
        /// @note Uses the Scenes and UI elements.
        /// @note The GUI will always render on top of the game, and will not change on scene changes.
        MIXED_WINDOW
    };
}

#endif // PANDORAUI_WINDOW_OPTIONS_HPP