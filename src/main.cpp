#include "PandoraUI/init.hpp"
#include "PandoraUI/window.hpp"
// #include "PandoraEX/list.hpp"
// #include "PandoraDebug/console.hpp"

// #include <windows.h>
// #include <iostream>
// using namespace std;

void init_window()
{
    PandoraUI::Window w;
    w.initialize();
    w.open();

    PandoraUI::Window w2;
    w2.initialize();
    w2.open();
}

using namespace PandoraEX;

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    PandoraUI::initialize();
    init_window();
    // system("pause");
    return PandoraUI::waitForExit(true);
}

