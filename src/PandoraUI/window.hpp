#ifndef PANDORAUI_WINDOW_HPP
#define PANDORAUI_WINDOW_HPP

#include "PandoraEX/object.hpp"
#include "IWindow.hpp"
#include "windowOpts.hpp"
#include "GUI/IElement.hpp"

namespace PandoraUI
{
    Class(Window) pextends IWindow
    {


        void render() override;
    public:
        const UsecaseType usecaseType = UsecaseType::GUI_WINDOW;
        const GPU_API gpuAPI = GPU_API::OPENGL;

        PandoraEX::AsyncList<std::reference_wrapper<GUI::IElement>> guiElements;

        Window() = default;

        void initialize() override;
    };
}

#endif // PANDORAUI_WINDOW_HPP