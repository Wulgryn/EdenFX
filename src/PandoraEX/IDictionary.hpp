#ifndef PANDORAEX_IDICTIONARY_HPP
#define PANDORAEX_IDICTIONARY_HPP

#include "PandoraEX/object.hpp"
#include "PandoraEX/list.hpp"
#include "PandoraEX/keyValuePair.hpp"

namespace PandoraEX
{
    template <typename Key, typename Value>
    Class(IDictionary)
    {
    public:
        virtual ~IDictionary() = default;

        /// @brief Adds a key-value pair to the dictionary.
        /// @param key The key of the pair.
        /// @param value The value of the pair.
        /// @throws `InvalidOperationException` - If the key already exists in the dictionary.
        virtual void add(const Key& key, const Value& value) = 0;

        /// @brief Removes a key-value pair from the dictionary by key.
        /// @param key The key of the pair to remove.
        /// @throws `NotFoundException` - If the key is not found in the dictionary.
        virtual void remove(const Key& key) = 0;

        /// @brief Clears all key-value pairs from the dictionary.
        virtual void clear() = 0;

        /// @brief Gets the value associated with a specific key.
        /// @param key The key of the pair to get.
        /// @return The value associated with the specified key.
        /// @throws `NotFoundException` - If the key is not found in the dictionary.
        virtual Value get(const Key& key) const = 0;

        /// @brief Gets the value associated with a specific key. If the key is not found, it inserts the default value and creates the key-value pair.
        /// @param key The key of the pair to get.
        /// @param defaultValue The default value to return if the key is not found.
        /// @return The value associated with the specified key, or the default value.
        virtual std::conditional_t<std::is_pointer_v<Value>, Value, Value&> get(const Key& key, const Value& defaultValue) = 0;

        /// @brief Gets the key-value pair associated with a specific key.
        /// @param key The key of the pair to get.
        /// @return The key-value pair associated with the specified key.
        /// @throws `NotFoundException` - If the key is not found in the dictionary.
        virtual KeyValuePair<Key, Value> getKeyValuePair(const Key& key) = 0;

        /// @brief Sets the value for a specific key.
        /// @param key The key of the pair to set.
        /// @param value The new value to associate with the key.
        /// @throws `NotFoundException` - If the key is not found in the dictionary.
        virtual void set(const Key& key, const Value& value) = 0;

        /// @brief Gets or sets the value associated with a specific key.
        /// @param key The key of the pair to get or set.
        /// @return The value associated with the specified key.
        virtual std::conditional_t<std::is_pointer_v<Value>, Value, Value&> operator[](const Key& key) = 0;

        /// @brief Gets the value associated with a specific key.
        /// @param key The key of the pair to get.
        /// @return The value associated with the specified key.
        virtual std::conditional_t<std::is_pointer_v<Value>, Value, const Value&> operator[](const Key& key) const = 0;

        /// @brief Checks if the dictionary contains a specific key.
        /// @param key The key to check for.
        /// @return True if the key is found in the dictionary, false otherwise.
        virtual bool containsKey(const Key& key) const = 0;

        /// @brief Checks if the dictionary contains a specific value.
        /// @param value The value to check for.
        /// @return True if the value is found in the dictionary, false otherwise.
        virtual bool containsValue(const Value& value) const = 0;

        /// @brief Gets a list of all keys in the dictionary.
        /// @return A list of all keys in the dictionary.
        virtual List<Key> keys() const = 0;

        /// @brief Gets a list of all values in the dictionary.
        /// @return A list of all values in the dictionary.
        virtual List<Value> values() const = 0;

        /// @brief Gets the number of key-value pairs in the dictionary.
        /// @return The number of pairs in the dictionary.
        virtual std::size_t size() const = 0;
    };
    }

#endif // PANDORAEX_IDICTIONARY_HPP