#ifndef PANDORAEX_COMPATIBILITY_UTILS_HPP
#define PANDORAEX_COMPATIBILITY_UTILS_HPP

#include "PandoraEX/object.hpp"

namespace PandoraEX::Compatibility::Utils
{
    /// @brief Compares two classes by their values (using universal_hash and equals).
    /// @tparam T The type of the classes to compare.
    /// @param a First class.
    /// @param b Second class.
    /// @return True if the classes are equal (byte-wise), false otherwise.
    template <class T>
        requires(std::is_class_v<T> || std::is_enum_v<T>)
    bool operator==(const T &a, const T &b)
    {
        return PandoraEX::equals(a, b) || PandoraEX::universal_hash(a) == PandoraEX::universal_hash(b);
    }
    // template <class T>
    // bool operator==(const T &a, const T &b)
    // {
    //     return equals(a, b) || universal_hash(a) == universal_hash(b);
    // }

    /// @brief Compares two classes by their values (using universal_hash and equals).
    /// @tparam T The type of the classes to compare.
    /// @param a First class.
    /// @param b Second class.
    /// @return True if the classes are equal (byte-wise), false otherwise.
    template <class T1, class T2>
        requires((std::is_class_v<T1> || std::is_enum_v<T1>) ||
                 (std::is_class_v<T2> || std::is_enum_v<T2>))
    bool operator==(const T1 &a, const T2 &b)
    {
        return PandoraEX::equals(a, b) || PandoraEX::universal_hash(a) == PandoraEX::universal_hash(b);
    }
    // template <class T1, class T2>
    // bool operator==(const T1 &a, const T2 &b)
    // {
    //     return equals(a, b) || universal_hash(a) == universal_hash(b);
    // }
}

#endif // PANDORAEX_COMPATIBILITY_UTILS_HPP