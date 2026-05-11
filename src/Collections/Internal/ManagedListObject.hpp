#ifndef COLLECTIONS_INTERNAL_MANAGEDLISTOBJECT_HPP
#define COLLECTIONS_INTERNAL_MANAGEDLISTOBJECT_HPP

#include "Core/Object/ManagedObject.hpp"
#include <vector>
#include <stdexcept>

namespace Eden::Collections::Internal
{
    template <typename T>
    class ManagedListObject : public Eden::Core::Object::ManagedObject
    {
    public:
        void Add(const T &value)
        {
            m_Data.push_back(value);
        }

        void RemoveAt(u32 index)
        {
            if (index >= m_Data.size())
            {
                throw std::out_of_range("List index out of range");
            }

            m_Data.erase(m_Data.begin() + index);
        }

        void Clear()
        {
            m_Data.clear();
        }

        u32 Count() const
        {
            return static_cast<u32>(m_Data.size());
        }

        T &At(u32 index)
        {
            if (index >= m_Data.size())
            {
                throw std::out_of_range("List index out of range");
            }

            return m_Data[index];
        }

        const T &At(u32 index) const
        {
            if (index >= m_Data.size())
            {
                throw std::out_of_range("List index out of range");
            }

            return m_Data[index];
        }

    private:
        std::vector<T> m_Data;
    };
}

#endif // COLLECTIONS_INTERNAL_MANAGEDLISTOBJECT_HPP