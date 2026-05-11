#ifndef CORE_OBJECT_MANAGEDOBJECT_HPP
#define CORE_OBJECT_MANAGEDOBJECT_HPP

#include <type_traits>
#include <concepts>
#include "Core/Object/Object.hpp"

namespace Eden::Core::Object
{
    template <typename T>
    concept HasClone = requires(const T *obj) {
        { obj->clone() } -> std::convertible_to<T *>;
    };

    template <typename TObject>
    class ManagedObject
    {
        TObject *m_object = nullptr;

    public:
        ManagedObject() : m_object(nullptr) {}

        ManagedObject(TObject *object) : m_object(object)
        {
            static_assert(std::derived_from<TObject, Object>, "TObject must derive from Object");
            if (m_object)
            {
                m_object->addRef();
            }
        }

        ~ManagedObject()
        {
            if (m_object)
            {
                m_object->release();
                m_object = nullptr;
            }
        }

        ManagedObject(const ManagedObject &other)
            : m_object(other.m_object)
        {
            if (m_object)
            {
                m_object->addRef();
            }
        }

        ManagedObject &operator=(const ManagedObject &other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (m_object != nullptr)
            {
                m_object->Release();
            }

            m_object = other.m_object;

            if (m_object != nullptr)
            {
                m_object->AddRef();
            }

            return *this;
        }

        ManagedObject<TObject> clone() const
        {
            if (!m_object)
            {
                return ManagedObject();
            }

            if constexpr (HasClone<TObject>)
            {
                return ManagedObject(m_object->clone());
            }
            else if constexpr (!std::is_abstract_v<TObject> && std::is_copy_constructible_v<TObject>)
            {
                return ManagedObject(new TObject(*m_object));
            }
            else
            {
                static_assert(HasClone<TObject>, "This type cannot be cloned. Add clone() or make it copy constructible.");
            }
        }

        TObject *operator->()
        {
            return m_object;
        }

        const TObject *operator->() const
        {
            return m_object;
        }

        TObject &get()
        {
            return *m_object;
        }

        const TObject &get() const
        {
            return *m_object;
        }

        bool isValid() const
        {
            return m_object != nullptr;
        }
    };
}

#endif // CORE_OBJECT_MANAGEDOBJECT_HPP