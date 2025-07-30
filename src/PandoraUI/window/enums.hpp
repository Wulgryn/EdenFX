#ifndef WINDOW_ENUMS_HPP
#define WINDOW_ENUMS_HPP

namespace PandoraUI
{
    enum class GPU_API
    {
        OPENGL,
        VULKAN,
        DIRECTX11,
        DIRECTX12
    };

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

#endif // WINDOW_ENUMS_HPP