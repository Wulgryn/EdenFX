#ifndef PANDORAEX_UNIQUE_LIST_HPP
#define PANDORAEX_UNIQUE_LIST_HPP

#include "list.hpp"

namespace PandoraEX
{
    /// @brief A list that only allows unique items.
    /// @tparam Type The type of the items in the list.
    /// @details This class inherits from List and overrides the add method to ensure that only unique items are added to the list.
    /// @note If the item is already present in the list, it will not be added again. 
    /// @see file:src/PandoraEX/list.hpp "list.hpp"
    template <class Type>
    class UniqueList : public List<Type>
    {
    public:
        /// @brief Add an item to the unique list.
        /// @note This method will only add the item if it is not already present in the list.
        /// @param item The item to be added to the unique list.
        void add(std::conditional_t<std::is_pointer_v<Type>, Type, const Type&> item) override
        {
            if (List<Type>::indexOf(item) == (size_t)-1)
                List<Type>::add(item);
        }
    };
}

#endif // PANDORAEX_UNIQUE_LIST_HPP