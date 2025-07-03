#ifndef UTILS_HPP
#define UTILS_HPP

#include "string.hpp"

namespace PandoraEX::Utils
{
    /// @brief Swaps two values of the same type.
    /// @param a The first value to swap.
    /// @param b The second value to swap.
    /// @note This function uses a temporary variable to swap the values.
    /// @note This is a simple utility function that can be used to swap any two values of the same type.
    /// @tparam Type The type of the values to swap.
    template <class Type>
    void swap(Type &a, Type &b)
    {
        Type temp = a;
        a = b;
        b = temp;
    }

    /// @brief Demangles a C++ name.
    /// @param name The mangled name to demangle.
    /// @return The demangled name.
    /// @note This function uses the `abi::__cxa_demangle` function from the C++ ABI library.
    /// @note If the demangling fails, it returns the original name.
    String demangle(const String name);
}

#endif // UTILS_HPP