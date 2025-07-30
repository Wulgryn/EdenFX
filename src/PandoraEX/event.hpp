#ifndef PANDORAEX_EVENT_HPP
#define PANDORAEX_EVENT_HPP

#include "list.hpp"
#include "method.hpp"

namespace PandoraEX
{
    /// @brief Event class for managing event callbacks.
    /// @tparam Args The types of the arguments that the event will pass to the callbacks.
    /// @details This class allows you to add, remove, and invoke methods that are registered to the event.
    template <typename... Args>
    class Event
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
        /// @tparam Args The types of the arguments that the methods expect.
        void Invoke(Args &&...args)
        {
            for (size_t i = 0; i < methods.size(); i++)
            {
                methods[i].Invoke(std::forward<Args>(args)...);
            }
        }
    };
}

#endif // PANDORAEX_EVENT_HPP