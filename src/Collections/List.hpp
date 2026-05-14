#ifndef EDEN_COLLECTIONS_LIST_HPP
#define EDEN_COLLECTIONS_LIST_HPP

#include "core/object/managed_object.hpp"

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <vector>

namespace Eden::Collections
{
    namespace Managed
    {
        template <class T>
        class ListData : public virtual OBJECT
        {
        public:
            std::vector<T> m_VecData;
        };
    }

    template <class T>
    class List : public MANAGEDOBJECT<Managed::ListData<T>>
    {
    public:
        using ValueType = T;
        using NativeVector = std::vector<T>;
        using Iterator = typename NativeVector::iterator;
        using ConstIterator = typename NativeVector::const_iterator;

        List(int size = 0)
        {
            this->data()->m_VecData.resize(size);
        }

        List(std::initializer_list<T> values)
        {
            this->data()->m_VecData = values;
        }

        explicit List(const NativeVector& values)
        {
            this->data()->m_VecData = values;
        }

        explicit List(NativeVector&& values)
        {
            this->data()->m_VecData = std::move(values);
        }

        void add(const T& item)
        {
            this->data()->m_VecData.push_back(item);
        }

        void push_back(const T& item)
        {
            add(item);
        }

        void insert(int index, const T& item)
        {
            if (index < 0)
            {
                index = 0;
            }

            if (index > size())
            {
                index = size();
            }

            this->data()->m_VecData.insert(this->data()->m_VecData.begin() + index, item);
        }

        T& operator[](int index)
        {
            return this->data()->m_VecData[index];
        }

        const T& operator[](int index) const
        {
            return this->data()->m_VecData[index];
        }

        T& at(int index)
        {
            return this->data()->m_VecData.at(static_cast<std::size_t>(index));
        }

        const T& at(int index) const
        {
            return this->data()->m_VecData.at(static_cast<std::size_t>(index));
        }

        T& first()
        {
            return this->data()->m_VecData.front();
        }

        const T& first() const
        {
            return this->data()->m_VecData.front();
        }

        T& last()
        {
            return this->data()->m_VecData.back();
        }

        const T& last() const
        {
            return this->data()->m_VecData.back();
        }

        int size() const
        {
            return static_cast<int>(this->data()->m_VecData.size());
        }

        int count() const
        {
            return size();
        }

        bool empty() const
        {
            return this->data()->m_VecData.empty();
        }


        void resize(int size)
        {
            this->data()->m_VecData.resize(static_cast<std::size_t>(size));
        }

        void reserve(int capacity)
        {
            this->data()->m_VecData.reserve(static_cast<std::size_t>(capacity));
        }

        void clear()
        {
            this->data()->m_VecData.clear();
        }

        int index_of(const T& item) const
        {
            auto it = std::find(this->data()->m_VecData.begin(), this->data()->m_VecData.end(), item);

            if (it != this->data()->m_VecData.end())
            {
                return static_cast<int>(std::distance(this->data()->m_VecData.begin(), it));
            }

            return -1;
        }

        bool contains(const T& item) const
        {
            return std::find(this->data()->m_VecData.begin(), this->data()->m_VecData.end(), item) != this->data()->m_VecData.end();
        }

        void remove_at(int index)
        {
            if (index >= 0 && index < size())
            {
                this->data()->m_VecData.erase(this->data()->m_VecData.begin() + index);
            }
        }

        bool remove(const T& item)
        {
            auto it = std::find(this->data()->m_VecData.begin(), this->data()->m_VecData.end(), item);

            if (it == this->data()->m_VecData.end())
            {
                return false;
            }

            this->data()->m_VecData.erase(it);
            return true;
        }

        Iterator begin()
        {
            return this->data()->m_VecData.begin();
        }

        ConstIterator begin() const
        {
            return this->data()->m_VecData.begin();
        }

        ConstIterator cbegin() const
        {
            return this->data()->m_VecData.cbegin();
        }

        Iterator end()
        {
            return this->data()->m_VecData.end();
        }

        ConstIterator end() const
        {
            return this->data()->m_VecData.end();
        }

        ConstIterator cend() const
        {
            return this->data()->m_VecData.cend();
        }
    };
}

#endif // EDEN_COLLECTIONS_LIST_HPP
