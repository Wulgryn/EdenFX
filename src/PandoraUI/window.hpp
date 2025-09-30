#ifndef PANDORAUI_WINDOW_HPP
#define PANDORAUI_WINDOW_HPP

#include "PandoraEX/object.hpp"
#include "IWindow.hpp"

namespace PandoraUI
{
    Class(Window) pextends IWindow
    {
    public:
        Window() = default;
    };
}

#endif // PANDORAUI_WINDOW_HPP