#ifndef PANDORAEX_EVENT_HPP
#define PANDORAEX_EVENT_HPP

#include "PandoraEX/object.hpp"
#include "list.hpp"
#include "method.hpp"

namespace PandoraEX
{

    /// @brief Event class for managing event callbacks.
    /// @tparam Args The types of the arguments that the event will pass to the callbacks.
    /// @details This class allows you to add, remove, and invoke methods that are registered to the event.
    template <typename... Args>
    Class(Event)
    {
        List<Method<void>> methods;

    public:
        /// @brief Default constructor for Event.
        /// @note Initializes an empty event with no registered methods.
        Event() = default;
        ~Event() = default;

        /// @brief Adds a method to the event.
        /// @param method The method to add.
        void operator+=(Method<void> method)
        {
            methods.add(method);
        }

        /// @brief Removes a method from the event.
        /// @param method The method to remove.
        void operator-=(Method<void> method)
        {
            methods.remove(method);
        }

        /// @brief Adds a method to the event.
        /// @param method The method to add.
        /// @details This is an alternative to the `+=` operator.
        void add(Method<void> method)
        {
            methods.add(method);
        }

        /// @brief Removes a method from the event.
        /// @param method The method to remove.
        /// @details This is an alternative to the `-=` operator.
        void remove(Method<void> method)
        {
            methods.remove(method);
        }

        /// @brief Clears all methods registered to the event.
        void clear()
        {
            methods.clear();
        }

        /// @brief Assignment operator.
        /// @param method The method to assign.
        /// @details This will clear the current methods and add the new method.
        /// @note This is useful for replacing the current event methods with a single method.
        void operator=(Method<void> method)
        {
            methods.clear();
            methods.add(method);
        }

        /// @brief Invokes all methods registered to the event with the provided arguments.
        /// @param args The arguments to pass to the methods.
        /// @details This will call each method in the order they were added to the event.
        void invoke(const Args &... args)
        {
            for (size_t i = 0; i < methods.size(); i++)
            {
                methods[i].invoke(args...);
            }
        }
    };

    /// @brief Controlled event class for managing event callbacks with ownership.
    /// @tparam Owner The type of the owner that controls the event.
    /// @tparam ...Args The types of the arguments that the event will pass to the callbacks.
    /// @tparam IsClearEnabled If true, all stored methods can be cleared by anyone.
    template <typename Owner, bool IsClearEnabled, typename... Args>
    class ControlledEvent;

    /// @brief Controlled event class for managing event callbacks with ownership.
    /// @tparam Owner The type of the owner that controls the event.
    /// @tparam ...Args The types of the arguments that the event will pass to the callbacks.
    /// @tparam IsClearEnabled If true, all stored methods can be cleared by anyone.
    template <typename Owner, typename... Args>
    Class(ControlledEvent<Owner, true, Args...>)
    {
        Event<Args...> event;
        friend Owner;

        // void invoke(Args &&...args)
        /// @brief Invokes all methods registered to the event with the provided arguments.
        /// @param args The arguments to pass to the methods.
        /// @details This will call each method in the order they were added to the event.
        /// @tparam Args The types of the arguments that the methods expect.
        void invoke(const Args &... args)
        {
            event.invoke(args...);
        }
    public:
        /// @brief Constructor for ControlledEvent.
        ControlledEvent() {}

        /// @brief Adds a method to the controlled event.
        /// @param method The method to add.
        void operator+=(Method<void> method)
        {
            event += method;
        }

        /// @brief Removes a method from the controlled event.
        /// @param method The method to remove.
        void operator-=(Method<void> method)
        {
            event -= method;
        }

        void add(Method<void> method)
        {
            event.add(method);
        }

        void remove(Method<void> method)
        {
            event.remove(method);
        }

        void operator=(Method<void> method)
        {
            event = method;
        }

        /// @brief Clears all methods registered to the controlled event.
        void clear()
        {
            event.clear();
        }
    };

    /// @brief Controlled event class for managing event callbacks with ownership.
    /// @tparam Owner The type of the owner that controls the event.
    /// @tparam ...Args The types of the arguments that the event will pass to the callbacks.
    /// @tparam IsClearEnabled If true, all stored methods can be cleared by anyone.
    template <typename Owner, typename... Args>
    Class(ControlledEvent<Owner, false, Args...>)
    {
        Event<Args...> event;
        friend Owner;

        /// @brief Invokes all methods registered to the event with the provided arguments.
        /// @param args The arguments to pass to the methods.
        /// @details This will call each method in the order they were added to the event.
        /// @tparam Args The types of the arguments that the methods expect.
        void invoke(Args &&...args)
        {
            event.invoke(std::forward<Args>(args)...);
        }
    public:
        /// @brief Constructor for ControlledEvent.
        ControlledEvent() {}

        /// @brief Adds a method to the controlled event.
        /// @param method The method to add.
        void operator+=(Method<void> method)
        {
            event += method;
        }

        /// @brief Removes a method from the controlled event.
        /// @param method The method to remove.
        void operator-=(Method<void> method)
        {
            event -= method;
        }
    };
}

#endif // PANDORAEX_EVENT_HPP