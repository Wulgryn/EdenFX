#include "window.hpp"

using namespace PandoraUI;

void Window::render()
{
    guiElements.forEach([](std::reference_wrapper<PandoraUI::GUI::IElement>& el) { el.get().render(); });
    guiElements.forEach([](std::reference_wrapper<PandoraUI::GUI::IElement>& el) { el.get().update(); });
    guiElements.validateRemovals();
}

void Window::initialize()
{
    IWindow::initialize();
    guiElements.onItemAdded += [this](std::reference_wrapper<PandoraUI::GUI::IElement>& el) 
    { 
        el.get().setParent(this);
        el.get().initialize(); 
    };
}