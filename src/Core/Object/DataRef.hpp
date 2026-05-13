#ifndef CORE_OBJECT_DATAREF_HPP
#define CORE_OBJECT_DATAREF_HPP

#include "Core/Object/ManagedObject.hpp"
#include <atomic>

namespace Eden::Core::Object
{
    template <typename TDataClass>
    class DataRef
    {
    public:
        std::atomic<long long> m_RefCount = 0;
        TDataClass m_Data;

        DataRef()
            : m_RefCount(1),
              m_Data()
        {
        }

        template <typename... Args>
        explicit DataRef(Args &&...args)
            : m_RefCount(1),
              m_Data(std::forward<Args>(args)...)
        {
        }
    };
}

#endif // CORE_OBJECT_DATAREF_HPP