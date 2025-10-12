#ifndef PANDORAEX_UTILS_HPP
#define PANDORAEX_UTILS_HPP

#include "string.hpp"

namespace PandoraEX::Utils
{
    namespace __helpers
    {
        template <class T>
        consteval const char *type_name_impl()
        {
#if defined(__GNUC__) || defined(__clang__)
            std::string_view p = __PRETTY_FUNCTION__; // "... T = <type> ..."
            auto s = p.find("T = ");
            s = (s == std::string_view::npos) ? 0 : s + 4;
            auto e = p.find_first_of(";]", s);
            auto name = p.substr(s, e - s);
            if (!name.empty() && (name.back() == ']' || name.back() == ';'))
                name.remove_suffix(1);
            return name.data();
            // return p.substr(s, e - s).data();
#else
            return "unsupported";
#endif
        }
    }
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

    template <class T>
    String typeName(T &&x)
    {
        (void)x;
        String tname = __helpers::type_name_impl<decltype(x)>();
        if (!tname.empty() && tname.back() == ']')
            tname.pop_back();
        return tname;
    }
}

#endif // PANDORAEX_UTILS_HPP