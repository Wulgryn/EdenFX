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
        const T *original_ptr = nullptr;
        std::shared_ptr<T> ptr;

    public:
        ValueWrapper() : ptr(std::make_shared<T>()){
            PDC::logTrace("ValueWrapper constructor(id): %d", id());
        };
        ValueWrapper(const T &value) : ptr(std::make_shared<T>(std::move(value))), original_ptr(std::addressof(value)) {
            
            if constexpr (std::is_base_of_v<Object, T>)
            {
                PDC::logTrace("<%s> ValueWrapper constructor(id): %d -> with value(id): %d", Utils::demangle(typeid(T).name()).c_str(), id(), ((Object)value).id());
            }
            else
            {
                PDC::logTrace("<%s> ValueWrapper constructor(id): %d -> not object value", Utils::demangle(typeid(T).name()).c_str(), id());
            }
            // PDC::logTrace("ValueWrapper constructor(id): %d -> with value(id): %d", id(), ((Object)value).id());
            if constexpr (std::is_base_of_v<Object, T> ) if(((Object)value).id() != ptr->id())
            {
                PDC::logFailure("original id: %d", ((Object)value).id());

                PDC::logFailure("wrapped id: %d", ptr->id());
            }
        }

        ValueWrapper(const ValueWrapper<T> &other) : Object(other), ptr(other.ptr), original_ptr(other.original_ptr)
        {
            if constexpr (std::is_base_of_v<Object, T>)
            {
                PDC::logTrace("<%s> ValueWrapper copy constructor(id): %d -> with value(id): %d", Utils::demangle(typeid(T).name()).c_str(), id(), ptr->id());
            }
            else
            {
                PDC::logTrace("<%s> ValueWrapper copy constructor(id): %d -> not object value", Utils::demangle(typeid(T).name()).c_str(), id());
            }
        }

        bool operator==(const ValueWrapper<T> &other) const
        {
            if (original_ptr == other.original_ptr)
                return true;
            if constexpr (std::is_pointer_v<T>)
            {
                if (!ptr || !other.ptr)
                {
                    return *ptr == *other.ptr;
                }
                return false;
            }
            else if constexpr (std::is_same_v<T, PandoraEX::Object>)
            {
                return *ptr == *other.ptr || ptr->equals(*other.ptr) || ptr->id() == other.ptr->id();
            }
            return ptr == other.ptr;
        }

        const T &get() const { return *ptr; }

        ~ValueWrapper()
        {
            if (ptr)
            {
                ptr.reset();
            }
        }
    };
}

#endif // PANDORAEX_VALUE_WRAPPER_HPP