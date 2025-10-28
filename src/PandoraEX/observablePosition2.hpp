#ifndef PANDORAEX_OBSERVABLEPOSITION2_HPP
#define PANDORAEX_OBSERVABLEPOSITION2_HPP

#include "PandoraEX/object.hpp"
#include "PandoraEX/property.hpp"

namespace PandoraEX
{
    Struct(ObservablePosition2)
    {
        Property<int> x = 0;
        Property<int> y = 0;

        ObservablePosition2() = default;
        ObservablePosition2(int xPos, int yPos) : x(xPos), y(yPos) {}

        void set(int xPos, int yPos, bool triggerEvent = true)
        {
            x.set(xPos, triggerEvent);
            y.set(yPos, triggerEvent);
        }

        bool operator==(const ObservablePosition2& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const ObservablePosition2& other) const
        {
            return !(*this == other);
        }
    };
}

#endif // PANDORAEX_OBSERVABLEPOSITION2_HPP