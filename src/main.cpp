#include "PandoraUI/init.hpp"
#include "PandoraUI/window.hpp"


int main( [[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    PandoraUI::initialize();
    PandoraUI::Window window;
    window.initialize();

    PandoraUI::Window window1;
    window1.initialize();

    PandoraUI::Window window2;
    window2.initialize();
    return PandoraUI::waitForExit();
}