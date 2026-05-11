#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

namespace EdenCore
{
    class Object
    {
        long long m_refCount = 0;
        
    public:
        void addRef()
        {
            ++m_refCount;
        }

        void release()
        {
            if (--m_refCount == 0)
            {
                delete this;
            }
        }

        long long refCount() const
        {
            return m_refCount;
        }
    };
}

#endif // CORE_OBJECT_HPP