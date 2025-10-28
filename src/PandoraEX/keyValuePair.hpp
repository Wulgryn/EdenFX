#ifndef PANDORAEX_KEYVALUEPAIR_HPP
#define PANDORAEX_KEYVALUEPAIR_HPP

#include "PandoraEX/object.hpp"

namespace PandoraEX
{
    template <typename Key, typename Value>
    Struct(KeyValuePair)
    {
        Key key;
        Value value;

        KeyValuePair() = default;
        KeyValuePair(const Key& k, const Value& v) : key(k), value(v) {}
        KeyValuePair(const KeyValuePair& other) : Object(other), key(other.key), value(other.value) {}
        KeyValuePair(KeyValuePair&& other) noexcept : Object(std::move(other)), key(std::move(other.key)), value(std::move(other.value)) {}
        KeyValuePair& operator=(KeyValuePair&& other) noexcept
        {
            if (this != &other)
            {
                key = std::move(other.key);
                value = std::move(other.value);
            }
            return *this;
        }
        KeyValuePair(const Key& k) : key(k), value(Value{}) {}


        bool operator==(const KeyValuePair& other) const
        {
            bool keyEqual = false;
            if constexpr (requires { { key == other.key } -> std::convertible_to<bool>; })
            {
                keyEqual = key == other.key;
            }
            else
            {
                keyEqual = PandoraEX::Compatibility::Utils::operator==(key, other.key);
            }
            bool valueEqual = false;
            if constexpr (requires { { value == other.value } -> std::convertible_to<bool>; })
            {
                valueEqual = value == other.value;
            }
            else
            {
                valueEqual = PandoraEX::Compatibility::Utils::operator==(value, other.value);
            }
            return (keyEqual && valueEqual) || this == &other;
        }

        bool operator!=(const KeyValuePair& other) const
        {
            return !(*this == other);
        }

        KeyValuePair& operator=(const KeyValuePair& other)
        {
            if (this != &other)
            {
                key = other.key;
                value = other.value;
            }
            return *this;
        }

        KeyValuePair& operator=(const Key& k)
        {
            key = k;
            value = Value{};
            return *this;
        }

        KeyValuePair& operator=(const Value& v)
        {
            value = v;
            return *this;
        }
    };
}

#endif // PANDORAEX_KEYVALUEPAIR_HPP