#ifndef PANDORAEX_OBSERVABLESIZE2_HPP
#define PANDORAEX_OBSERVABLESIZE2_HPP

#include "PandoraEX/object.hpp"
#include "PandoraEX/property.hpp"
namespace PandoraEX
{
    Struct(ObservableSize2)
    {
        Property<int> width = 0;
        Property<int> height = 0;

        ObservableSize2() = default;
        ObservableSize2(int w, int h) : width(w), height(h) {}

        void set(int w, int h, bool triggerEvent = true)
        {
            width.set(w, triggerEvent);
            height.set(h, triggerEvent);
        }

        bool operator==(const ObservableSize2& other) const
        {
            return width == other.width && height == other.height;
        }

        bool operator!=(const ObservableSize2& other) const
        {
            return !(*this == other);
        }
    };
}

#endif // PANDORAEX_OBSERVABLESIZE2_HPP