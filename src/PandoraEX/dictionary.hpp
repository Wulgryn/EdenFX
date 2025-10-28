#ifndef PANDORAEX_DICTIONARY_HPP
#define PANDORAEX_DICTIONARY_HPP

#include "PandoraEX/object.hpp"
#include "PandoraEX/IDictionary.hpp"
#include <type_traits>

namespace PandoraEX
{
    template <typename Key, typename Value>
    Class(Dictionary) extends public IDictionary<Key, Value>, public IEnumerable<KeyValuePair<Key, Value>>
    {
    protected:
        List<Key> _keys;
        List<Value> _values;

    public:
        Dictionary() = default;
        virtual ~Dictionary() = default;

        virtual void add(const Key &key, const Value &value) override
        {
            if (containsKey(key))
                ThrowExceptionF(Exceptions::InvalidOperationException, "Key already exists in dictionary.");
            _keys.add(key);
            _values.add(value);
        }

        virtual void remove(const Key &key) override
        {
            size_t index = _keys.indexOf(key);
            if (index == (size_t)-1)
                ThrowExceptionF(Exceptions::NotFoundException, "Key not found in dictionary.");
            _keys.removeAt(index);
            _values.removeAt(index);
        }

        virtual void clear() override
        {
            _keys.clear();
            _values.clear();
        }

        virtual Value get(const Key &key) const override
        {
            size_t index = _keys.indexOf(key);
            if (index == (size_t)-1)
                ThrowExceptionF(Exceptions::NotFoundException, "Key not found in dictionary.");
            if constexpr (std::is_pointer<Value>::value)
            {
                return _values.at(index);
            }
            else
            {
                return const_cast<Value &>(_values.at(index));
            }
        }

        virtual std::conditional_t<std::is_pointer_v<Value>, Value, Value &> get(const Key &key, const Value &defaultValue) override
        {
            size_t index = _keys.indexOf(key);
            if (index == (size_t)-1)
            {
                add(key, defaultValue);
                index = _keys.size() - 1;
            }
            if constexpr (std::is_pointer<Value>::value)
                return const_cast<Value>(_values.at(index));
            else
            return const_cast<Value &>(_values.at(index));
        }

        virtual KeyValuePair<Key, Value> getKeyValuePair(const Key &key) override
        {
            size_t index = _keys.indexOf(key);
            if (index == (size_t)-1)
                ThrowExceptionF(Exceptions::NotFoundException, "Key not found in dictionary.");
            return KeyValuePair<Key, Value>(_keys.at(index), _values.at(index));
        }

        virtual void set(const Key &key, const Value &value) override
        {
            size_t index = _keys.indexOf(key);
            if (index == (size_t)-1)
                ThrowExceptionF(Exceptions::NotFoundException, "Key not found in dictionary.");
            if constexpr (std::is_pointer<Value>::value)
            {
                if (_values[index] != nullptr)
                    delete _values[index];
                _values.replaceAt(index, value);
            }
            else
                const_cast<Value &>(_values[index]) = value;
        }

        virtual std::conditional_t<std::is_pointer_v<Value>, Value, Value &> operator[](const Key &key) override
        {
            size_t index = _keys.indexOf(key);

            if (index == (size_t)-1)
                ThrowExceptionF(Exceptions::NotFoundException, "Key not found in dictionary.");

            if constexpr (std::is_pointer<Value>::value)
                return _values[index];
            else
            return const_cast<Value &>(_values[index]);
        }

        virtual std::conditional_t<std::is_pointer_v<Value>, Value, const Value &> operator[](const Key &key) const override
        {
            size_t index = _keys.indexOf(key);
            if (index == (size_t)-1)
                ThrowExceptionF(Exceptions::NotFoundException, "Key not found in dictionary.");
            return _values.at(index);
        }

        virtual bool containsKey(const Key &key) const override
        {
            return _keys.contains(key);
        }

        virtual bool containsValue(const Value &value) const override
        {
            return _values.contains(value);
        }

        virtual List<Key> keys() const override
        {
            return _keys;
        }

        virtual List<Value> values() const override
        {
            return _values;
        }

        virtual std::size_t size() const override
        {
            return _keys.size();
        }

        virtual KeyValuePair<Key, Value> &begin() override
        {
            if (_keys.size() == 0)
                ThrowExceptionF(Exceptions::IndexOutOfBoundsException, "Dictionary is empty.");
            return *new KeyValuePair<Key, Value>(_keys[0], _values[0]);
        }

        virtual KeyValuePair<Key, Value> &end() override
        {
            if (_keys.size() == 0)
                ThrowExceptionF(Exceptions::IndexOutOfBoundsException, "Dictionary is empty.");
            return *new KeyValuePair<Key, Value>(_keys[_keys.size() - 1], _values[_values.size() - 1]);
        }
        virtual IEnumerable<KeyValuePair<Key, Value>> &reverse() override
        {
            _keys.reverse();
            _values.reverse();
            return *this;
        }

        virtual IEnumerable<KeyValuePair<Key, Value>> &reverse(Method<bool> compare) override
        {
            // Create a vector of indices
            std::vector<size_t> indices(_keys.size());
            for (size_t i = 0; i < indices.size(); ++i)
                indices[i] = i;

            // Sort indices based on the comparison of key-value pairs
            std::sort(indices.begin(), indices.end(), [&](size_t a, size_t b)
                      { return compare.invoke(KeyValuePair<Key, Value>(_keys[a], _values[a]), KeyValuePair<Key, Value>(_keys[b], _values[b])); });

            // Create new lists for keys and values
            List<Key> sortedKeys;
            List<Value> sortedValues;

            for (size_t i = 0; i < indices.size(); ++i)
            {
                sortedKeys.add(_keys[indices[i]]);
                sortedValues.add(_values[indices[i]]);
            }
            _keys = sortedKeys;
            _values = sortedValues;
            _keys.reverse();
            _values.reverse();
            return *this;
        }

        virtual IEnumerable<KeyValuePair<Key, Value>> &sort() override
        {
            // Create a vector of indices
            std::vector<size_t> indices(_keys.size());
            for (size_t i = 0; i < indices.size(); ++i)
                indices[i] = i;

            // Sort indices based on the comparison of key-value pairs
            std::sort(indices.begin(), indices.end(), [&](size_t a, size_t b)
                      { return _keys[a] < _keys[b]; });

            // Create new lists for keys and values
            List<Key> sortedKeys;
            List<Value> sortedValues;

            for (size_t i = 0; i < indices.size(); ++i)
            {
                sortedKeys.add(_keys[indices[i]]);
                sortedValues.add(_values.at(indices[i]));
            }

            // Assign back to original lists
            _keys = sortedKeys;
            _values = sortedValues;

            return *this;
        }

        virtual IEnumerable<KeyValuePair<Key, Value>> &sort(Method<bool> compare) override
        {
            // Create a vector of indices
            std::vector<size_t> indices(_keys.size());
            for (size_t i = 0; i < indices.size(); ++i)
                indices[i] = i;

            // Sort indices based on the comparison of key-value pairs
            std::sort(indices.begin(), indices.end(), [&](size_t a, size_t b)
                      { return compare.invoke(KeyValuePair<Key, Value>(_keys[a], _values[a]), KeyValuePair<Key, Value>(_keys[b], _values[b])); });

            // Create new lists for keys and values
            List<Key> sortedKeys;
            List<Value> sortedValues;

            for (size_t i = 0; i < indices.size(); ++i)
            {
                sortedKeys.add(_keys[indices[i]]);
                sortedValues.add(_values[indices[i]]);
            }

            // Assign back to original lists
            _keys = sortedKeys;
            _values = sortedValues;

            return *this;
        }

        template <class Type = IEnumerable<KeyValuePair<Key, Value>>>
        IEnumerable<Type> select(Method<Type> selector)
        {
            List<Type> new_enumerable;
            for (size_t i = 0; i < _keys.size(); i++)
            {
                new_enumerable.add(selector.invoke(KeyValuePair<Key, Value>(_keys[i], _values[i])));
            }
            return new_enumerable;
        }

        IEnumerable<KeyValuePair<Key, Value>> &where(Method<bool> predicate) override
        {
            List<KeyValuePair<Key, Value>> &new_enumerable = *new List<KeyValuePair<Key, Value>>();
            for (size_t i = 0; i < _keys.size(); i++)
            {
                KeyValuePair<Key, Value> kvp(_keys[i], _values[i]);
                if (predicate.invoke(kvp))
                    new_enumerable.add(kvp);
            }
            return new_enumerable;
        }

        void forEach(Method<void> action) override
        {
            for (size_t i = 0; i < _keys.size(); i++)
            {
                action.invoke(KeyValuePair<Key, Value>(_keys[i], _values[i]));
            }
        }
    };
}
#endif // PANDORAEX_DICTIONARY_HPP