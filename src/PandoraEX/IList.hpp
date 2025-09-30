#ifndef PANDORAEX_ILIST_HPP
#define PANDORAEX_ILIST_HPP

#include "PandoraEX/object.hpp"
#include <cstddef>

namespace PandoraEX {

template <typename T>
Class(IList) {
public:
    virtual ~IList() = default;

    /// @brief Adds an item to the list.
    /// @param item The item to add.
    virtual void add(const T& item) = 0;

    /// @brief Removes an item from the list.
    /// @param item The item to remove.
    virtual void remove(const T& item) = 0;

    /// @brief Removes an item at a specific index from the list.
    /// @param index The index of the item to remove.
    virtual void removeAt(std::size_t index) = 0;

    /// @brief Clears the list.
    virtual void clear() = 0;

    /// @brief Gets an item at a specific index from the list.
    /// @param index The index of the item to get.
    /// @return The item at the specified index.
    virtual const T& at(std::size_t index) const = 0;

    /// @brief Gets an item at a specific index from the list.
    /// @param index The index of the item to get.
    /// @return The item at the specified index.
    virtual const T& operator[](std::size_t index) const = 0;


    /// @brief Gets the index of an item in the list.
    /// @param item The item to find.
    /// @return The index of the item in the list, or -1 if the item is not found.
    virtual std::size_t indexOf(const T& item) const = 0;

    /// @brief Checks if the list contains a specific item.
    /// @param item The item to check for.
    /// @return True if the item is found in the list, false otherwise.
    virtual bool contains(const T& item) const = 0;

    /// @brief Gets the size of the list.
    /// @return The size of the list.
    virtual std::size_t size() const = 0;
};

} // namespace PandoraEX

#endif // PANDORAEX_ILIST_HPP