#ifndef CORE_OBJECT_MANAGEDOBJECT_HPP
#define CORE_OBJECT_MANAGEDOBJECT_HPP

#include <type_traits>
#include <concepts>
#include <utility>
#include "core/object/object.hpp"
#include "core/object/data_ref.hpp"
// #include "core/array_wrapper.hpp"

namespace Eden::Core
{
    template <class T>
    class ArrayWrapper;
}

namespace Eden::Core::Object
{

    template <class TObjectClass, class TDataClass>
    class ManagedObject : public virtual Object
    {
    private:
        DataRef<TDataClass> *m_DataRef = nullptr;

    protected:
        struct ForwardToDataRef {};

        ManagedObject() : m_DataRef(new DataRef<TDataClass>()) {}

        template <typename... Args>
        explicit ManagedObject(ForwardToDataRef, Args &&...args)
            : m_DataRef(new DataRef<TDataClass>(std::forward<Args>(args)...))
        {
        }

        TDataClass *data()
        {
            return &m_DataRef->m_Data;
        }

        const TDataClass *data() const
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

        static ArrayWrapper<TObjectClass> NewArray(int size)
        {
            return ArrayWrapper<TObjectClass>(size);
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

#if defined(AUTO_USINGS)
using namespace Eden::Core::Object;
#endif

#define MANAGEDOBJECT Eden::Core::Object::ManagedObject

#endif // CORE_OBJECT_MANAGEDOBJECT_HPP