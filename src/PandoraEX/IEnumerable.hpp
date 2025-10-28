#ifndef PANDORAEX_IENUMERABLE_HPP
#define PANDORAEX_IENUMERABLE_HPP

#include "PandoraEX/object.hpp"
#include "method.hpp"

namespace PandoraEX
{
    template <typename T>
    Class(IEnumerable)
    {
    public:
        /// @brief The begin iterator of the enumerable.
        /// @return A reference to the first element in the enumerable.
        /// @throws 'IndexOutOfBoundsException' - If the enumerable is empty.
        virtual T &begin() = 0;
        /// @brief The end iterator of the enumerable.
        /// @return A reference to the last element in the enumerable.
        /// @throws 'IndexOutOfBoundsException' - If the enumerable is empty.
        virtual T &end() = 0;

        /// @brief Reverses the order of the elements in the enumerable.
        /// @return A reference to the enumerable itself.
        virtual IEnumerable<T> &reverse() = 0;
        /// @brief Reverses the order of the elements in the enumerable using a custom comparison function.
        /// @param compare A function that takes two elements and returns true if the first element should
        /// @return A reference to the enumerable itself.
        virtual IEnumerable<T> &reverse(Method<bool> compare) = 0;
        /// @brief Sorts the elements in the enumerable.
        /// @return A reference to the enumerable itself.
        virtual IEnumerable<T> &sort() = 0;
        /// @brief Sorts the elements in the enumerable using a custom comparison function.
        /// @param compare A function that takes two elements and returns true if the first element should
        /// @return A reference to the enumerable itself.
        virtual IEnumerable<T> &sort(Method<bool> compare) = 0;

        /// @brief Projects each element of the enumerable into a new form.
        /// @tparam Type The type of the projected enumerable. Default is IEnumerable<T>.
        /// @param selector A function that takes an element and returns the projected value.
        /// @return A reference to the projected enumerable.
        template <class Type = IEnumerable<T>>
        Type select(Method<T> selector) { return Type(); }

        /// @brief Filters the elements of the enumerable based on a predicate.
        /// @param predicate A function that takes an element and returns true if the element should be included.
        /// @return A reference to the filtered enumerable.
        virtual IEnumerable<T> &where(Method<bool> predicate) = 0;

        /// @brief Performs the specified action on each element of the enumerable.
        /// @param action A function that takes an element and performs an action on it.
        virtual void foreach(Method<void> action) = 0;
    };
}

#endif // PANDORAEX_IENUMERABLE_HPP