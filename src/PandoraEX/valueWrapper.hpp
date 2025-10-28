#ifndef PANDORAEX_VALUE_WRAPPER_HPP
#define PANDORAEX_VALUE_WRAPPER_HPP

#include <memory>
#include <type_traits>
#include "PandoraEX/object.hpp"
#include "exception.hpp"
#include "PandoraDebug/console.hpp"

namespace PandoraEX
{
    template <class T>
    Class(ValueWrapper)
    {
        std::shared_ptr<T> ptr_;

    public:
        using element_type = T;

        // 1) Default: üres (absztraktnál elengedhetetlen)
        ValueWrapper()
        {
            if constexpr (std::default_initializable<T> && !std::is_abstract_v<T>)
            {
                ptr_ = std::make_shared<T>(); // csak ha lehet default-olni és nem absztrakt
            }
            // különben nullptr marad
        }

        // 2) Közvetlen shared_ptr átvétel
        explicit ValueWrapper(std::shared_ptr<T> p) : ptr_(std::move(p)) {}

        // 3) Leszármazottból építés (Button -> ValueWrapper<IElement>)
        template <class U>
            requires std::derived_from<std::decay_t<U>, T> // C++20 concept
        ValueWrapper(U && u)
            : ptr_(std::make_shared<std::decay_t<U>>(std::forward<U>(u)))
        {
        } // make_shared: egy allokáció, osztott tulajdon
        // (derived_from és perfect forwarding: l. cppreference) :contentReference[oaicite:1]{index=1}

        // 4) In-place konstrukció megadott konkrét típussal és paraméterekkel
        template <class U, class... Args>
            requires std::derived_from<U, T>
        explicit ValueWrapper(std::in_place_type_t<U>, Args && ...args)
            : ptr_(std::make_shared<U>(std::forward<Args>(args)...))
        {
        } // make_shared részletek :contentReference[oaicite:2]{index=2}

        // pointer-szerű API
        T *operator->() const noexcept { return ptr_.get(); }
        T &operator*() const noexcept { return *ptr_; }
        explicit operator bool() const noexcept { return static_cast<bool>(ptr_); }

        // kiegészítők
        std::shared_ptr<T> get() const noexcept { return ptr_; }
        void reset() noexcept { ptr_.reset(); }
    };
}

#endif // PANDORAEX_VALUE_WRAPPER_HPP