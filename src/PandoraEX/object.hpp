#ifndef PANDORAEX_OBJECT_HPP
#define PANDORAEX_OBJECT_HPP

#include <string>
#include <optional>
#include <type_traits>
#include <concepts>
#include <compare>
#include <memory>
#include <tuple>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>
#include <functional>
#include <typeinfo>
#include <atomic>

namespace std
{
    template <typename T>
    struct atomic;
}

namespace PandoraEX
{
    template <class T>
    bool equals(const T &a, const T &b);
    template <class T1, class T2>
    bool equals(const T1 &a, const T2 &b);

    template <class T>
    std::uint64_t universal_hash(const T &);

    class Object
    {
        static std::atomic<unsigned long long> s_nextID;
        unsigned long long _id;

    public:
        std::string typeName = "Object";
        Object();
        // Object(std::string typeName);
        Object(const Object &other);
        // Object(Object &&other) noexcept;

        Object &operator=(const Object &other);
        // Object &operator=(Object &&other) noexcept;

        unsigned long long id() const;

        virtual ~Object();

        virtual std::string getTypeName() const;

        virtual bool isType(const std::string &_typeName) const;

        virtual bool isType(const Object &other) const;

        bool operator==(const Object &other) const;
        bool operator!=(const Object &other) const;
        bool equals(const Object &other) const;

        // template <class T>
        // bool operator==(const T &other) const
        // {
        //     // return PandoraEX::operator==(*this, other);

        //     // if constexpr (requires { { *this == other } -> std::convertible_to<bool>; })
        //     // {
        //     //     return *this == other || (PandoraEX::equals(*this, other) || PandoraEX::universal_hash(*this) == PandoraEX::universal_hash(other));
        //     // }
        //     return PandoraEX::equals(*this, other) || PandoraEX::universal_hash(*this) == PandoraEX::universal_hash(other);
            
        // }

        template <class T>
        bool operator!=(const T &other) const
        {
            return !(*this == other);
        }

        template <class T>
        bool equals(const T &other) const
        {
            return PandoraEX::equals(*this, other);
        }

        std::strong_ordering operator<=>(const Object &other) const;
    };
    template <typename Object>
    concept ObjectType = std::derived_from<std::remove_cvref_t<Object>, Object>;

#define __DEPAREN(X) __ESC(__ISH X)
#define __ISH(...)   __ISH __VA_ARGS__
#define __ESC(...)   __ESC_(__VA_ARGS__)
#define __ESC_(...)  __VAN ## __VA_ARGS__
#define __VAN__ISH   /* üres */

/// @brief Macro to define a class that extends Object.
/// @param className The name of the class.
/// @param ... Optional base classes to extend first.
#define Class(className, ...) class __DEPAREN(className) : __VA_ARGS__ __VA_OPT__(,) public virtual PandoraEX::Object
/// @brief Macro to define a struct that extends Object.
/// @param structName The name of the struct.
/// @param ... Optional base classes to extend first.
#define Struct(structName, ...) struct __DEPAREN(structName) : __VA_ARGS__ __VA_OPT__(,) public virtual PandoraEX::Object
    // #define extends(...) , ##__VA_ARGS__

#define extends ,
#define pextends , public
#define public public
    // ---------- FNV-1a 64-bit (egyszerű, gyors, nem kriptográfiai) ----------
    static constexpr std::uint64_t FNV_OFFSET = 14695981039346656037ull;
    static constexpr std::uint64_t FNV_PRIME = 1099511628211ull;

    constexpr std::uint64_t fnv1a_append(std::uint64_t h, std::byte b) noexcept
    {
        h ^= static_cast<std::uint8_t>(b);
        h *= FNV_PRIME;
        return h;
    }
    constexpr std::uint64_t fnv1a_bytes(const void *data, std::size_t n, std::uint64_t h = FNV_OFFSET) noexcept
    {
        auto p = static_cast<const std::byte *>(data);
        for (std::size_t i = 0; i < n; ++i)
            h = fnv1a_append(h, p[i]);
        return h;
    }
    // Forrás: FNV-1a leírás (nem kriptográfiai, de táblákhoz jó). :contentReference[oaicite:1]{index=1}

    // ---------- Hash-combine ----------
    constexpr std::uint64_t hash_combine(std::uint64_t h, std::uint64_t v) noexcept
    {
        // XOR + FNV lépés: olcsó kombinálás
        h ^= v + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2);
        h *= FNV_PRIME;
        return h;
    }

    // ---------- Alaptípusok hash-elése ----------
    template <class T>
    concept HasStdHash = requires(const T &t) {
        { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>;
    };

    template <class T>
    concept TriviallyByteHashable = std::is_trivially_copyable_v<T> && (!std::is_pointer_v<T>);

    template <class T>
    concept IsTupleLike = requires {
        std::tuple_size<std::remove_reference_t<T>>::value;
    };

    template <class T>
    concept HasBeginEnd = requires(T t) {
        { std::begin(t) } -> std::input_or_output_iterator;
        { std::end(t) };
    };

    template <template <class...> class Template, class T>
    struct is_specialization_of : std::false_type
    {
    };

    template <template <class...> class Template, class... Args>
    struct is_specialization_of<Template, Template<Args...>> : std::true_type
    {
    };

    template <class T>
    inline constexpr bool is_optional_v = is_specialization_of<std::optional, std::remove_cvref_t<T>>::value;

    // Előre deklaráció
    template <class T>
    std::uint64_t universal_hash(const T &);

    // Tuple-like (std::tuple, std::pair, std::array, saját tuple-like) kezelése
    template <class Tup, std::size_t... I>
    std::uint64_t hash_tuple_impl(const Tup &tp, std::index_sequence<I...>)
    {
        std::uint64_t h = FNV_OFFSET;
        ((h = hash_combine(h, universal_hash(std::get<I>(tp)))), ...);
        return h;
    }

    template <IsTupleLike Tup>
    std::uint64_t hash_tuple(const Tup &tp)
    {
        constexpr std::size_t N = std::tuple_size_v<std::remove_reference_t<Tup>>;
        return hash_tuple_impl(tp, std::make_index_sequence<N>{});
    }

    // Konténerek (sorrend-érzékeny)
    template <HasBeginEnd Range>
    std::uint64_t hash_range(const Range &r)
    {
        std::uint64_t h = FNV_OFFSET;
        for (const auto &e : r)
            h = hash_combine(h, universal_hash(e));
        return h;
    }

    // Opciók
    template <class T>
    std::uint64_t hash_optional(const std::optional<T> &o)
    {
        std::uint64_t h = FNV_OFFSET;
        if (!o)
            return hash_combine(h, 0xDEADBEAFu);
        return hash_combine(h, universal_hash(*o));
    }

    // Pointer — címet hash-eljük (nem dereferálunk)
    template <class T>
    std::uint64_t hash_ptr(T *p)
    {
        auto v = reinterpret_cast<std::uintptr_t>(p);
        return fnv1a_bytes(&v, sizeof(v));
    }

    namespace detail
    {

        struct Any
        {
            template <class U>
            constexpr operator U() const noexcept
                requires std::is_default_constructible_v<U>
            {
                return U{};
            }
        };

        template <class T, std::size_t N>
        consteval bool aggregate_init_of_n()
        {
            if constexpr (!std::is_aggregate_v<T>)
            {
                return false;
            }
            else
            {
                return []<std::size_t... I>(std::index_sequence<I...>)
                {
                    if constexpr (requires { T{(static_cast<void>(I), Any{})...}; })
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }(std::make_index_sequence<N>{});
            }
        }

        template <class T, std::size_t N>
        concept AggregateInitOfN = aggregate_init_of_n<T, N>();

        template <class T, std::size_t Lo, std::size_t Hi>
        consteval std::size_t count_fields_binsearch()
        {
            if constexpr (Lo == Hi)
            {
                return Lo;
            }
            else
            {
                constexpr std::size_t Mid = (Lo + Hi + 1) / 2;
                if constexpr (AggregateInitOfN<T, Mid>)
                {
                    return count_fields_binsearch<T, Mid, Hi>();
                }
                else
                {
                    return count_fields_binsearch<T, Lo, Mid - 1>();
                }
            }
        }

        template <class T>
        consteval std::size_t field_count()
        {
            if constexpr (!std::is_aggregate_v<T>)
            {
                return 0;
            }
            // Feltételezett max mezőszám (igény szerint növelhető)
            return count_fields_binsearch<T, 0, 16>();
        }

        // N mezőre tie() – structured binding nem variadikus, ezért generálunk esete(ke)t.
        template <class T>
        auto aggregate_tie_impl(T &, std::integral_constant<std::size_t, 0>)
        {
            return std::tie();
        }

#define PFR_TIE_CASE(N, ...)                                              \
    template <class T>                                                    \
    auto aggregate_tie_impl(T &t, std::integral_constant<std::size_t, N>) \
    {                                                                     \
        auto &&[__VA_ARGS__] = t;                                         \
        return std::tie(__VA_ARGS__);                                     \
    }

        PFR_TIE_CASE(1, a0)
        PFR_TIE_CASE(2, a0, a1)
        PFR_TIE_CASE(3, a0, a1, a2)
        PFR_TIE_CASE(4, a0, a1, a2, a3)
        PFR_TIE_CASE(5, a0, a1, a2, a3, a4)
        PFR_TIE_CASE(6, a0, a1, a2, a3, a4, a5)
        PFR_TIE_CASE(7, a0, a1, a2, a3, a4, a5, a6)
        PFR_TIE_CASE(8, a0, a1, a2, a3, a4, a5, a6, a7)
        PFR_TIE_CASE(9, a0, a1, a2, a3, a4, a5, a6, a7, a8)
        PFR_TIE_CASE(10, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)
        PFR_TIE_CASE(11, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
        PFR_TIE_CASE(12, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)
        PFR_TIE_CASE(13, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)
        PFR_TIE_CASE(14, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)
        PFR_TIE_CASE(15, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)
        PFR_TIE_CASE(16, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)

#undef PFR_TIE_CASE

        template <class T>
        auto aggregate_tie(T &t)
        {
            constexpr std::size_t N = field_count<std::remove_cvref_t<T>>();
            return aggregate_tie_impl(t, std::integral_constant<std::size_t, N>{});
        }

    } // namespace detail

    // ---------- universal_hash: általános típusokra ----------
    template <class T>
    std::uint64_t universal_hash(const T &v)
    {
        // 1) Tuple-like
        if constexpr (IsTupleLike<T>)
        {
            // Típusnév (instabil, implementációfüggő) + elemek
            std::uint64_t h = fnv1a_bytes(typeid(T).name(), std::strlen(typeid(T).name())); // :contentReference[oaicite:3]{index=3}
            return hash_combine(h, hash_tuple(v));
        }
        // 2) std::optional
        else if constexpr (is_optional_v<T>)
        {
            std::uint64_t h = fnv1a_bytes(typeid(T).name(), std::strlen(typeid(T).name()));
            return hash_combine(h, hash_optional(v));
        }
        // 3) Range (konténer)
        else if constexpr (HasBeginEnd<T> && !std::is_convertible_v<T, std::string_view>)
        {
            std::uint64_t h = fnv1a_bytes(typeid(T).name(), std::strlen(typeid(T).name()));
            return hash_combine(h, hash_range(v));
        }
        // 4) Pointer
        else if constexpr (std::is_pointer_v<T>)
        {
            return hash_ptr(v);
        }
        // 5) Van std::hash<T>
        else if constexpr (HasStdHash<T>)
        {
            std::uint64_t h = fnv1a_bytes(typeid(T).name(), std::strlen(typeid(T).name()));
            return hash_combine(h, static_cast<std::uint64_t>(std::hash<T>{}(v)));
        }
        // 6) Aggregátum (mezőértékek kibontása — N<=16 mező)
        else if constexpr (std::is_aggregate_v<T>)
        {
            // FIGYELEM: MEZŐNEVEKET NEM TUDUNK LEKÉRNI C++23-BAN. Csak ÉRTÉKEK mennek itt. :contentReference[oaicite:4]{index=4}
            auto tup = detail::aggregate_tie(const_cast<T &>(v)); // tie of references
            std::uint64_t h = fnv1a_bytes(typeid(T).name(), std::strlen(typeid(T).name()));
            return hash_combine(h, hash_tuple(tup));
        }
        // 7) Triviálisan másolható — byte-hash
        else if constexpr (TriviallyByteHashable<T>)
        {
            std::uint64_t h = fnv1a_bytes(typeid(T).name(), std::strlen(typeid(T).name()));
            return fnv1a_bytes(&v, sizeof(T), h);
        }
        // 8) Végső fallback: cím (nagyon ritkán ide jutunk)
        else
        {
            return hash_ptr(&v);
        }
    }

    /// @brief Compares two classes by their addresses.
    /// @tparam T The type of the classes to compare.
    /// @param a First class.
    /// @param b Second class.
    /// @return True if the classes are the same instance (same address), false otherwise.
    template <class T>
    bool equals(const T &a, const T &b)
    {
        return std::addressof(a) == std::addressof(b);
    }

    /// @brief Compares two classes by their addresses.
    /// @tparam T The type of the classes to compare.
    /// @param a First class.
    /// @param b Second class.
    /// @return True if the classes are the same instance (same address), false otherwise.
    template <class T1, class T2>
    bool equals(const T1 &a, const T2 &b)
    {
        return static_cast<const void *>(std::addressof(a)) == static_cast<const void *>(std::addressof(b));
    }

    
}

#endif // PANDORAEX_OBJECT_HPP