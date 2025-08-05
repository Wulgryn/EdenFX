#ifndef PANDORAEX_ASYNC_LIST_HPP
#define PANDORAEX_ASYNC_LIST_HPP

#include "object.hpp"
#include "uniqueList.hpp"

namespace PandoraEX
{
    /// @brief Asynchronous list that supports delayed removal of items.
    /// @tparam Type The type of items in the list.
    template <class Type>
    Class(AsyncList) pextends List<Type>
    // class AsyncList : public List<Type>
    {
        UniqueList<int> removeList;
        bool __clear = false;

    public:
        /// @brief Removes an item at the specified index.
        /// @param index The index of the item to remove.
        /// @note This method will not immediately remove the item, only on validation.
        void removeAt(size_t index) override
        {
            removeList.add(index);
        }

        /// @brief Removes an item from the list.
        /// @param item The item to remove.
        /// @note This method will not immediately remove the item, only on validation.
        void remove(const Type &item) override
        {
            removeList.add(List<Type>::indexOf(item));
        }

        /// @brief Clears the list.
        /// @note This method will not immediately clear the list, only on validation.
        void clear() override
        {
            __clear = true;
        }

        /// @brief Validates and applies any pending removals.
        /// @throws `IndexOutOfBoundsException` - If any index in the remove list is out of bounds.
        void validateRemovals()
        {
            if (__clear)
                List<Type>::clear();
            else
                for (size_t i = 0; i < removeList.size(); i++)
                    List<Type>::removeAt(removeList[i]);
            __clear = false;
            removeList.clear();
        }
    };
}

#endif // PANDORAEX_ASYNC_LIST_HPP