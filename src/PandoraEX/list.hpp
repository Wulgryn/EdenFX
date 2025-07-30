#ifndef LIST_HPP
#define LIST_HPP

#include <vector>
#include <iostream>

// #include "refSmartWrap.hpp"
#include "valueWrapper.hpp"
#include "exception.hpp"

namespace PandoraEX
{
    template <class Type>
    class List
    {
    protected:
        // std::vector<RefSmartWrap<Type>> data_vec;

        std::vector<ValueWrapper<Type>> data_vec;

    public:
        /// @brief Default constructor for List.
        /// @param size Initial size of the list. Default is 0.
        List(size_t size = 0)
        {
            data_vec.reserve(size);
        }
/* REVIEW: add with &&
 *$ *===============================REVIEW==================================
 *$ * WHY: Optimization
 *$ * DESCRIPTION: Maybe RefSmartWrap is not needed if we are passing rvalue references, also if this not needed it means faster code and less memory usage.
 *$ * HOW: add(Type &&item) { data_vec.push_back(std::move(item)); } but the other methods are need to change too, like indexOf <- == op
 *$ *=======================================================================
 *$ */

        /// @brief Adds an item to the list.
        /// @details If Type is a pointer, it will add the pointer directly. If Type is not a pointer, it will create a new object on the heap and add it to the list.
        /// @tparam Type The type of the item to be added.
        /// @param item The item to be added to the list.
        virtual void add(std::conditional_t<std::is_pointer_v<Type>, Type, const Type&> item)
        {
            // if (typeid(RefSmartWrap<Type>) == typeid(Type))
            //     data_vec.push_back(item);
            // else
            //     data_vec.push_back(RefSmartWrap<Type>(std::make_shared<Type>(item)));

            // data_vec.push_back(ValueWrapper<Type>(item));
            data_vec.emplace_back(item);
        }

        /// @brief Finds the index of an item in the list.
        /// @param item The item to find in the list.
        /// @return The index of the item in the list, or -1 if the item is not found.
        size_t indexOf(std::conditional_t<std::is_pointer_v<Type>, Type, const Type&> item) const
        {
            for (size_t i = 0; i < data_vec.size(); i++)
            {
                if (data_vec[i] == item)
                    return i;
            }
            return -1;
        }

        /// @brief Removes an item from the list.
        /// @param item The item to remove from the list.
        /// @throws `NotFoundException` - If the item is not found in the list
        virtual void remove(const Type &item)
        {
            size_t index = indexOf(item);
            if (index == (size_t)-1)
                ThrowExceptionF(Exceptions::NotFoundException, "Item not found in list.");
            data_vec.erase(data_vec.begin() + index);
        }

        /// @brief Removes an item at a specific index from the list.
        /// @param index The index of the item to remove from the list.
        /// @throws `IndexOutOfBoundsException` - If the index is out of bounds.
        virtual void removeAt(size_t index)
        {
            if (index >= data_vec.size())
                ThrowException(Exceptions::IndexOutOfBoundsException, "Index out of bounds. Got " + std::string(index + "") + ", expected 0-" + std::string((data_vec.size() - 1) + "") + ".");
            data_vec.erase(data_vec.begin() + index);
        }

        /// @brief Clears the list.
        virtual void clear()
        {
            data_vec.clear();
        }

        /// @brief Gets the size of the list.
        /// @return The size of the list.
        size_t size() const
        {
            return data_vec.size();
        }

        /// @brief Gets the item at a specific index in the list.
        /// @throws `IndexOutOfBoundsException` - If the index is out of bounds.
        /// @tparam Type The type of the item to get from the list.
        /// @param index The index of the item to get from the list.
        /// @note If Type is a pointer, the returned value will be a pointer to the item. If Type is not a pointer, the returned value will be a reference to the item.
        /// @return A reference to the item at the specified index in the list.
        std::conditional_t<std::is_pointer_v<Type>, Type, Type&> operator[](size_t index) const
        {
            if (index >= data_vec.size())
                ThrowExceptionF(Exceptions::IndexOutOfBoundsException, "Index out of bounds. Got {}, expected 0-{}.",std::to_string(index),std::to_string(data_vec.size() - 1));
            return data_vec[index].get();
        }

        /// @brief Gets the item at a specific index in the list.
        /// @throws `IndexOutOfBoundsException` - If the index is out of bounds.
        /// @tparam Type The type of the item to get from the list.
        /// @param index The index of the item to get from the list.
        /// @note If Type is a pointer, the returned value will be a pointer to the item. If Type is not a pointer, the returned value will be a reference to the item.
        /// @return A reference to the item at the specified index in the list.
        std::conditional_t<std::is_pointer_v<Type>, Type, Type&> at(size_t index) const
        {
            if (index >= data_vec.size())
                ThrowExceptionF(Exceptions::IndexOutOfBoundsException, "Index out of bounds. Got {}, expected 0-{}.",std::to_string(index),std::to_string(data_vec.size() - 1));
            return data_vec[index].get();
        }

        /// @brief Casts the list to a new list with a different type.
        /// @tparam ToType The type to cast the list to.
        /// @return A new list with the specified type containing the items from the original list.
        /// @note This method will convert each item in the list to the specified type using static_cast.
        template <class ToType>
        List<ToType> cast() const
        {
            List<ToType> new_list;
            for (size_t i = 0; i < data_vec.size(); i++)
            {
                new_list.add(static_cast<ToType>(data_vec[i].get()));
            }
            return new_list;
        }

        /// @brief Casts the list to a new list with a different type.
        /// @tparam ToType The type to cast the list to.
        /// @return A new list with the specified type containing the items from the original list.
        /// @note This method will convert each item in the list to the specified type using static_cast.
        template <class ToType>
        List<ToType> toList() const
        {
            List<ToType> new_list;
            for (size_t i = 0; i < data_vec.size(); i++)
            {
                new_list.add(static_cast<ToType>(data_vec[i].get()));
            }
            return new_list;
        }

        /// @brief Prints the items in the list.
        /// @details It will print the address of each item in the list.
        virtual void print() const
        {
            for (size_t i = 0; i < data_vec.size(); i++)
            {
                if constexpr (std::is_pointer_v<Type>)
                {
                    std::cout << data_vec[i].get() << std::endl;
                }
                else
                {
                    std::cout << &data_vec[i].get() << std::endl;
                }
            }
        }

        /// @brief Default destructor for List.
        /// @note It will clear the list and free the memory used by the items in the list.
        virtual ~List()
        {
            clear();
        }
    };
}

#endif // LIST_HPP