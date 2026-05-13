#ifndef EDEN_COLLECTIONS_LIST_HPP
#define EDEN_COLLECTIONS_LIST_HPP

#include "Core/Object/ManagedObject.hpp"
#include <vector>

namespace Eden::Collections
{
    namespace Managed
    {
        template <typename T, class B>
        class ListData
        {
        public:
            std::vector<T> m_VecData;
        };
    }

    template <typename T>
    class List : public ManagedObject<Managed::ListData>
    {
    public:

        List(int size = 0)
        {
            data()->m_VecData.resize(size);
        }
    };
}


#endif // EDEN_COLLECTIONS_LIST_HPP