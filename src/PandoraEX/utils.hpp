#ifndef PANDORAEX_UTILS_HPP
#define PANDORAEX_UTILS_HPP

#include "string.hpp"
#include <format>

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

        namespace detail
        {

            // --- automatikus "kibontás" a log/format számára ---

            // alapértelmezett: hagyjuk a típust
            template <class T>
            constexpr decltype(auto) unwrap(T &&v) { return std::forward<T>(v); }

            // std::string -> string_view (std::format barát)
            inline std::string_view unwrap(const std::string &s) { return std::string_view{s}; }

            // Van get() -> annak értéke (általános Property-re is jó)
            template <class P>
            concept has_get = requires(const P &p) { p.get(); };

            template <has_get P>
            inline auto unwrap(const P &p) { return p.get(); }

            // --- printf -> {} átalakító (egyszerű, a legtöbb %d/%f/%s-esetet lefedi) ---
            inline std::string printf_to_braces(std::string_view in)
            {
                std::string out;
                out.reserve(in.size() + 16);
                size_t i = 0;

                auto isdig = [](char c)
                { return std::isdigit(static_cast<unsigned char>(c)); };

                while (i < in.size())
                {
                    if (in[i] != '%')
                    {
                        out.push_back(in[i++]);
                        continue;
                    }

                    // "%%" -> "%"
                    if (i + 1 < in.size() && in[i + 1] == '%')
                    {
                        out.push_back('%');
                        i += 2;
                        continue;
                    }

                    size_t j = i + 1;

                    // FLAGS
                    bool left = false, plus = false, space = false, alt = false, zero = false;
                    while (j < in.size())
                    {
                        char f = in[j];
                        if (f == '-')
                        {
                            left = true;
                            ++j;
                        }
                        else if (f == '+')
                        {
                            plus = true;
                            ++j;
                        }
                        else if (f == ' ')
                        {
                            space = true;
                            ++j;
                        }
                        else if (f == '#')
                        {
                            alt = true;
                            ++j;
                        }
                        else if (f == '0')
                        {
                            zero = true;
                            ++j;
                        }
                        else
                            break;
                    }
                    if (left)
                        zero = false; // printf: '-' felülírja a '0' paddingot

                    // WIDTH
                    bool width_dyn = false;
                    int width = 0;
                    if (j < in.size() && in[j] == '*')
                    {
                        width_dyn = true;
                        ++j;
                    }
                    else
                    {
                        while (j < in.size() && isdig(in[j]))
                        {
                            width = width * 10 + (in[j] - '0');
                            ++j;
                        }
                    }

                    // PRECISION
                    bool prec_present = false, prec_dyn = false;
                    int prec = 0;
                    if (j < in.size() && in[j] == '.')
                    {
                        prec_present = true;
                        ++j;
                        if (j < in.size() && in[j] == '*')
                        {
                            prec_dyn = true;
                            ++j;
                        }
                        else
                        {
                            while (j < in.size() && isdig(in[j]))
                            {
                                prec = prec * 10 + (in[j] - '0');
                                ++j;
                            }
                        }
                    }

                    // LENGTH (eldobjuk)
                    while (j < in.size())
                    {
                        char l = in[j];
                        if (l == 'h' || l == 'l' || l == 'j' || l == 'z' || l == 't' || l == 'L')
                        {
                            ++j;
                        }
                        else
                            break;
                    }

                    if (j >= in.size())
                    {
                        out.push_back('%');
                        i = j;
                        continue;
                    }
                    char spec = in[j++];

                    // SPEC → std::format típus
                    char type = 0;
                    switch (spec)
                    {
                    case 'd':
                    case 'i':
                    case 'u':
                        type = 'd';
                        break; // 'u' nincs std::formatban -> 'd'
                    case 'o':
                        type = 'o';
                        break;
                    case 'x':
                    case 'X':
                        type = spec;
                        break;
                    case 'f':
                    case 'F':
                        type = 'f';
                        break;
                    case 'e':
                    case 'E':
                    case 'g':
                    case 'G':
                    case 'a':
                    case 'A':
                        type = spec;
                        break;
                    case 'c':
                        type = 'c';
                        break;
                    case 's':
                        type = 's';
                        break;
                    case 'p':
                        type = 'p';
                        break;
                    default:
                        type = 0;
                        break; // ismeretlen -> alap "{}"
                    }

                    // Építsd a "{:...}" specifikációt
                    out.push_back('{');
                    out.push_back(':');

                    // előjel
                    if (plus)
                        out.push_back('+');
                    else if (space)
                        out.push_back(' ');

                    // alternatív formátum (#)
                    if (alt)
                        out.push_back('#');

                    // igazítás / 0-padding
                    if (left)
                    {
                        out.push_back('<');
                    }
                    else if (zero)
                    {
                        out.push_back('0');
                    }

                    // szélesség
                    if (width_dyn)
                        out += "{}";
                    else if (width > 0)
                        out += std::to_string(width);

                    // precízió
                    if (prec_present)
                    {
                        out.push_back('.');
                        if (prec_dyn)
                            out += "{}";
                        else
                            out += std::to_string(prec);
                    }

                    // típus
                    if (type)
                        out.push_back(type);

                    out.push_back('}');
                    i = j;
                }
                return out;
            }

        } // namespace PandoraEX::Utils::detail

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

    template <class... Args>
    inline std::string format(std::string_view fmt, Args &&...args)
    {
        const bool has_braces = (fmt.find('{') != std::string_view::npos) && (fmt.find('}') != std::string_view::npos);
        const bool has_percent = (fmt.find('%') != std::string_view::npos);

        // 1) {}-os: közvetlen std::format
        if (has_braces)
        {
            return std::vformat(fmt,
                                std::make_format_args(__helpers::detail::unwrap(std::forward<Args>(args))...));
        }

        // 2) %-os: átkonvertáljuk {}-ra, majd std::format
        if (has_percent)
        {
            const std::string converted = __helpers::detail::printf_to_braces(fmt);
            return std::vformat(converted,
                                std::make_format_args(__helpers::detail::unwrap(std::forward<Args>(args))...));
        }

        // 3) nincs placeholder
        return std::string(fmt);
    }
}

#endif // PANDORAEX_UTILS_HPP