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
        virtual T &begin() = 0;
        /// @brief The end iterator of the enumerable.
        /// @return A reference to the last element in the enumerable.
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
    };
}

#endif // PANDORAEX_IENUMERABLE_HPP