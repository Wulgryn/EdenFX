#ifndef PANDORAEX_PROPERTY_HPP
#define PANDORAEX_PROPERTY_HPP

#include "PandoraEX/object.hpp"
#include "string.hpp"
#include "event.hpp"
#include "valueWrapper.hpp"

namespace PandoraEX
{
    template <typename T, bool IsReadonly = false, class Ownder = void>
    class Property;

    template <typename T, class Owner>
    Class(Property<T, false, Owner>)
    {
        T _value;

    public:
        /// @brief Event triggered when the property value changes.
        /// @param newValue The new value of the property.
        /// @param oldValue The old value of the property.
        ControlledEvent<Property<T, false, Owner>, true, T, T> onChange;

        /// @brief Default constructor for the Property class.
        /// @details Initializes the property with a default value.
        Property() {}

        /// @brief Constructor for the Property class.
        /// @param value The initial value of the property.
        Property(const T &value) : _value(value) {}

        /// @brief Gets the current value of the property.
        /// @return The current value of the property.
        T get() const { return _value; }

        operator T()
        {
            return _value;
        }

        T* operator->()
        {
            return &_value;
        }

        /// @brief Sets the value of the property.
        /// @param value The new value to set.
        /// @details This will trigger the `onChange` event if the value has changed.
        void set(const T &value)
        {
            if(_value != value) 
            {
                T oldValue = _value;
                _value = value;
                onChange.invoke(_value, oldValue);
            }
        }

        /// @brief Assignment operator.
        /// @param value The new value to assign.
        /// @details This will trigger the `onChange` event if the value has changed.
        Property &operator=(const T &value)
        {
            set(value);
            return *this;
        }
    };

    template <typename T, class Owner>
    Class(Property<T, true, Owner>)
    {
        T _value;

        /// @brief Sets the value of the property.
        /// @param value The new value to set.
        /// @param triggerEvent If true, the `onChange` event will be triggered if the value has changed. (default is true)
        void set(const T &value, bool triggerEvent = true)
        {
            if(_value != value) 
            {
                T oldValue = _value;
                _value = value;
                if (triggerEvent) onChange.invoke(_value, oldValue);
            }
        }

        /// @brief Assignment operator.
        /// @param value The new value to assign.
        /// @details This will trigger the `onChange` event if the value has changed.
        Property &operator=(const T &value)
        {
            set(value);
            return *this;
        }
        friend Owner;
    public:
        /// @brief Event triggered when the property value changes.
        /// @param newValue The new value of the property.
        /// @param oldValue The old value of the property.
        ControlledEvent<Property<T, true, Owner>, true, T, T> onChange;

        /// @brief Constructor for the Property class.
        /// @param value The initial value of the property.
        Property(const T &value) : _value(value) {}

        /// @brief Gets the current value of the property.
        /// @return The current value of the property.
        T get() const { return _value; }

        operator T()
        {
            return _value;
        }

        T* operator->()
        {
            return &_value;
        }
    };
}

#endif // PANDORAEX_PROPERTY_HPP