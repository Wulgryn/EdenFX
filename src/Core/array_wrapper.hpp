#ifndef EDEN_CORE_ARRAY_WRAPPER_HPP
#define EDEN_CORE_ARRAY_WRAPPER_HPP

#include "core/object/managed_object.hpp"

namespace Eden::Core
{
    namespace Managed
    {
        template <class T>
        class ArrayWrapperData : public virtual OBJECT
        {
        public:
            T *m_Array = nullptr;
            int m_Size = 0;

            ~ArrayWrapperData()
            {
                delete[] m_Array;
            }
        };
    }

    template <class T>
    class ArrayWrapper : public MANAGEDOBJECT<ArrayWrapper<T>, Managed::ArrayWrapperData<T>>
    {
    public:
        ArrayWrapper(int size = 0)
        {
            if (size < 0)
            {
                size = 0;
            }

            this->data()->m_Array = new T[size];
            this->data()->m_Size = size;
        }

        int size() const
        {
            return this->data()->m_Size;
        }

        T &operator[](int index)
        {
            return this->data()->m_Array[index];
        }

        const T &operator[](int index) const
        {
            return this->data()->m_Array[index];
        }

        T *raw_array()
        {
            return this->data()->m_Array;
        }

        const T *raw_array() const
        {
            return this->data()->m_Array;
        }
    };
}

#endif // EDEN_CORE_ARRAY_WRAPPER_HPP