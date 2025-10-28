#ifndef PANDORAUI_GUI_BUTTON_HPP
#define PANDORAUI_GUI_BUTTON_HPP

#include "PandoraEX/object.hpp"
#include "IElement.hpp"
#include "PandoraEX/size2.hpp"
#include "PandoraEX/position2.hpp"

namespace PandoraUI::GUI
{
    Class(Button) extends public IElement
    {
    public:
        PandoraEX::Size2 size = PandoraEX::Size2(200, 100);
        PandoraEX::Position2 position = PandoraEX::Position2(0, 0);


        void initialize() override;
        void update() override;
    };
}

#endif // PANDORAUI_GUI_BUTTON_HPP