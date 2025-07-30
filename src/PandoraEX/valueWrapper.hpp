#ifndef PANDORAEX_VALUE_WRAPPER_HPP
#define PANDORAEX_VALUE_WRAPPER_HPP

#include <memory>
#include <type_traits>
#include "exception.hpp"

namespace PandoraEX
{
    template <class T>
    class ValueWrapper
    {
        using Type = std::conditional_t<std::is_pointer_v<std::remove_reference_t<T>>, std::remove_reference_t<T>, std::remove_reference_t<T> *>;
        Type value;
        std::conditional_t<std::is_pointer_v<T>, T, const T&> refVal = value;
        std::unique_ptr<std::remove_reference_t<T>> ptr = nullptr; // Pointer to the object if T is not a pointer.
    public:
        /// @brief Flag to indicate whether to free the memory when the ValueWrapper is destroyed.
        /// @details If T is a pointer, this flag will be used to determine whether to free the memory pointed to by the pointer. If T is not a pointer, this flag will be ignored.
        bool freeAll = true; // Flag to indicate whether to free the memory when the ValueWrapper is destroyed.

        /// @brief Default constructor for ValueWrapper.
        ValueWrapper() : value(nullptr) {};

        /// @brief Constructor for ValueWrapper that takes a value.
        /// @param value The value to be wrapped.
        ValueWrapper(const T &value) : refVal(value)
        {
            if constexpr (std::is_pointer_v<T>)
            {
                // If T is a pointer, assign it directly.
                this->value = value;
            }
            else
            {
                // Otherwise, allocate a new object.
                ptr = std::make_unique<std::remove_reference_t<T>>(value);
                // std::shared_ptr<std::remove_reference_t<T>> ptr_shared = std::make_shared<std::remove_reference_t<T>>(value); // Ensure the object is allocated on the heap.
                this->value = ptr.get(); // Get the raw pointer from the shared_ptr.
                // std::cout << "ValueWrapper: Allocated new object at " << this->value << std::endl;
                // std::cout << "ValueWrapper: Value is " << *this->value << std::endl;
            }
        }

        /// @brief Move constructor for ValueWrapper.
        // ValueWrapper(ValueWrapper &&other) noexcept
        //     : value(other.value), ptr(std::move(other.ptr)), freeAll(other.freeAll)
        // {
        //     other.value = nullptr;
        //     other.freeAll = true;
        // }

        ValueWrapper(ValueWrapper &&) noexcept = default;
        ValueWrapper &operator=(ValueWrapper &&) noexcept = default;

        // --- másoló konstruktor ---
        ValueWrapper(const ValueWrapper &other)
            : freeAll(other.freeAll)
        {
            if constexpr (std::is_pointer_v<T>)
            {
                value = other.value;
            }
            else
            {
                if (other.ptr)
                {
                    ptr = std::make_unique<std::remove_reference_t<T>>(*other.ptr);
                    value = ptr.get();
                }
                else
                {
                    ptr = nullptr;
                    value = nullptr;
                }
            }
        }

        // --- másoló értékadó operátor ---
        ValueWrapper &operator=(const ValueWrapper &other)
        {
            if (this != &other)
            {
                freeAll = other.freeAll;
                if constexpr (std::is_pointer_v<T>)
                {
                    value = other.value;
                }
                else
                {
                    if (other.ptr)
                    {
                        ptr = std::make_unique<std::remove_reference_t<T>>(*other.ptr);
                        value = ptr.get();
                    }
                    else
                    {
                        ptr.reset();
                        value = nullptr;
                    }
                }
            }
            return *this;
        }

        /// @brief Funcion to get the wrapped value.
        /// @details If T is a pointer, it returns the pointer. If T is not a pointer, it returns a reference to the value.
        /// @return The wrapped value.
        /// @throws `NullReferenceException` - If the value is null and T is a pointer.
        std::conditional_t<std::is_pointer_v<T>, T, T &> get() const
        {
            if constexpr (std::is_pointer_v<T>)
            {
                if (!value)
                {
                    ThrowExceptionF(PandoraEX::Exceptions::NullReferenceException, "Value is null.");
                }
                return value;
            }
            else
            {
                return *value;
            }
        }

        bool operator==(const ValueWrapper<T> &other) const
        {
            if constexpr (std::is_pointer_v<T>)
            {
                return value == other.value || &refVal == &other.refVal;
            }
            else
            {
                return &value == &other.value || &refVal == &other.refVal;
            }
        }

        bool isPointer() const
        {
            return std::is_pointer_v<T>;
        }

        ~ValueWrapper() noexcept
        {
            if constexpr (!std::is_pointer_v<T>)
            {
                if (ptr)
                {
                    ptr.reset();
                    ptr = nullptr;
                }
            }
            else
            {
                if (freeAll && value)
                {
                    delete value;    // Free the memory if freeAll is true.
                    value = nullptr; // Set the pointer to null.
                }
            }
        }
    };
}
#endif // PANDORAEX_VALUE_WRAPPER_HPP