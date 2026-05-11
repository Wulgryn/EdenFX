#ifndef COLLECTIONS_LIST_HPP
#define COLLECTIONS_LIST_HPP

#include "Core/Object/ManagedObject.hpp"
#include "Internal/ManagedListObject.hpp"

namespace Eden::Collections
{
    template <typename T>
    class List
    {
    public:
        List()
            : m_Object(new Eden::Collections::Internal::ManagedListObject<T>())
        {
        }

        void Add(const T &value)
        {
            m_Object->Add(value);
        }

        void RemoveAt(u32 index)
        {
            m_Object->RemoveAt(index);
        }

        void Clear()
        {
            m_Object->Clear();
        }

        u32 Count() const
        {
            return m_Object->Count();
        }

        T &operator[](u32 index)
        {
            return m_Object->At(index);
        }

        const T &operator[](u32 index) const
        {
            return m_Object->At(index);
        }

    private:
        Eden::Core::Object::ManagedObject<Eden::Collections::Internal::ManagedListObject<T>> m_Object;
    };
}

#endif // COLLECTIONS_LIST_HPP