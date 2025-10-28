#include "PandoraUI/init.hpp"
#include "PandoraUI/window.hpp"
#include "PandoraUI/GUI/button.hpp"

int main( [[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    PandoraUI::initialize();

    PandoraUI::Window window;
    window.initialize();
    PandoraUI::GUI::Button button;
    button.position = PandoraEX::Position2(100, 100);
    button.size = PandoraEX::Size2(200, 100);
    window.guiElements.add(button);

    PandoraUI::Window window1;
    window1.initialize();

    window1.guiElements.add(button);
    // window1.guiElements.add(*(new PandoraUI::GUI::Button()));
    // window1.guiElements.add(CreateElement(PandoraUI::GUI::Button));

    PandoraUI::Window window2;

    window2.initialize();

    PandoraUI::GUI::Button button2;
    button2.position = PandoraEX::Position2(200, 200);
    button2.size = PandoraEX::Size2(100, 200);
    window2.guiElements.add(button2);

    PandoraUI::Window window3;
    window3.initialize();

    
    
    // PandoraUI::WindowManager::closeAllWindows();

    return PandoraUI::waitForExit();
}