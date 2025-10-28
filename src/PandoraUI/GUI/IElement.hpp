#ifndef PANDORAUI_GUI_IGUIELEMENT_HPP
#define PANDORAUI_GUI_IGUIELEMENT_HPP

#include "PandoraEX/object.hpp"
#include "PandoraEX/asyncList.hpp"
#include "PandoraEX/property.hpp"
#include "PandoraEX/size2.hpp"

#include "PandoraUI/rendering/shader.hpp"

namespace PandoraEX
{
    class Size2;
    template <typename Key, typename Value>
    class Dictionary;
}

namespace PandoraUI::GUI
{
    Class(IElement)
    {
        PandoraEX::Dictionary<unsigned int, Object*> _contextParents;
    protected:
        PandoraEX::Size2 __getViewPort();

    public:
        PandoraEX::AsyncList<Shader*> shaders;
        // PandoraEX::Property<Object*> parent;
        IElement();

        void setParent(Object* parent);
        Object* getParent() const;


        virtual void initialize();
        virtual void render();
        virtual void update() = 0;
    };

#define CreateElement(ElementClass) *(new ElementClass())
}


#endif // PANDORAUI_GUI_IGUIELEMENT_HPP