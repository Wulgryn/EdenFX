#ifndef CORE_OBJECT_MANAGEDOBJECT_HPP
#define CORE_OBJECT_MANAGEDOBJECT_HPP

#include <type_traits>
#include <concepts>
#include <utility>
#include "Core/Object/Object.hpp"
#include "Core/Object/DataRef.hpp"

namespace Eden::Core::Object
{

    template <typename TObject>
    class ManagedObject : public virtual Object
    {
    private:
        DataRef<TObject> *m_DataRef = nullptr;

    protected:
        struct ForwardToDataRef {};

        ManagedObject() : m_DataRef(new DataRef<TObject>()) {}

        template <typename... Args>
        explicit ManagedObject(ForwardToDataRef, Args &&...args)
            : m_DataRef(new DataRef<TObject>(std::forward<Args>(args)...))
        {
        }

        TObject *data()
        {
            return &m_DataRef->m_Data;
        }

        const TObject *data() const
        {
            return &m_DataRef->m_Data;
        }

    public:
        ManagedObject(const ManagedObject &other) : m_DataRef(other.m_DataRef)
        {
            addRef();
        }

        ManagedObject& operator=(const ManagedObject& other)
        {
            if (this == &other)
            {
                return *this;
            }

            release();

            m_DataRef = other.m_DataRef;

            addRef();

            return *this;
        }

        ManagedObject(ManagedObject&& other) noexcept
            : m_DataRef(other.m_DataRef)
        {
            other.m_DataRef = nullptr;
        }

        ManagedObject& operator=(ManagedObject&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            release();

            m_DataRef = other.m_DataRef;
            other.m_DataRef = nullptr;

            return *this;
        }

        ~ManagedObject()
        {
            release();
        }

        long long getRefCount() const
        {
            if (m_DataRef == nullptr)
            {
                return 0;
            }

            return m_DataRef->m_RefCount.load();
        }

    private:
        void addRef()
        {
            if(m_DataRef) m_DataRef->m_RefCount.fetch_add(1);
        }

        void release()
        {
            if(!m_DataRef) return;
            if (m_DataRef->m_RefCount.fetch_sub(1) == 1)
            {
                delete m_DataRef;
            }
            m_DataRef = nullptr;
        }
    };
}

#define ManagedDataClass(className) namespace Managed { class className##Data

#define ManagedObject Eden::Core::Object::ManagedObject

#endif // CORE_OBJECT_MANAGEDOBJECT_HPP