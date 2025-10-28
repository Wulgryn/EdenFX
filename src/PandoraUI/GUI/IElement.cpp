#include "IElement.hpp"

#include "PandoraDebug/console.hpp"

#include "PandoraUI/IWindow.hpp"

using namespace PandoraDebug;
using namespace PandoraUI::GUI;
using namespace PandoraEX;

PandoraEX::Size2 IElement::__getViewPort()
{
    if (getParent() != nullptr && getParent()->getTypeName() != "NullParent")
    {
        if (dynamic_cast<PandoraUI::IWindow*>(getParent()) != nullptr)
        {
            PandoraUI::IWindow* window = dynamic_cast<PandoraUI::IWindow*>(getParent());
            return PandoraEX::Size2(window->size.width.get(), window->size.height.get());
        }
        else if (dynamic_cast<GUI::IElement*>(getParent()) != nullptr)
        {
            GUI::IElement* element = dynamic_cast<GUI::IElement*>(getParent());
            return element->__getViewPort();
        }
        else
        {
            DC::logWarning("Parent is not a recognized type for getting viewport size. Falling back to default size.");
        }
    }
    DC::logWarning("Element has no valid parent. Falling back to default size.");
    return PandoraEX::Size2(800, 600); // Default size if no parent
}

IElement::IElement()
{
    Object* parentPtr = new Object();
    parentPtr->typeName = "NullParent";
    setParent(parentPtr);
}

void IElement::setParent(Object* parentObj)
{
    if(!_contextParents.containsKey(WindowManager::getCurrentWindowContext()))
    {
        _contextParents.add(WindowManager::getCurrentWindowContext(), parentObj);
    }
    else
    {
        _contextParents.set(WindowManager::getCurrentWindowContext(), parentObj);
    }
}

Object* IElement::getParent() const
{
    if(_contextParents.containsKey(WindowManager::getCurrentWindowContext()))
    {
        return _contextParents[WindowManager::getCurrentWindowContext()];
    }
    return nullptr;
}

void IElement::initialize()
{
    // shaders.onItemAdded += [](Shader* const& shader) { shader->initialize(); };
    // shaders.forEach([](Shader*& shader) {  shader->initialize(); });
}

void IElement::render()
{
    shaders.forEach([](Shader*& shader) { shader->use(); });
    shaders.validateRemovals();
}