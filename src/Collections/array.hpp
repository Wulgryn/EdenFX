#ifndef EEN_COLLECTIONS_ARRAY_HPP
#define EEN_COLLECTIONS_ARRAY_HPP

#include "core/object/managed_object.hpp"

namespace Eden::Collections
{
    namespace Managed
    {
        template <class T>
        class ArrayData : public virtual OBJECT
        {
        public:
            
        };
    }

    template <class T>
    class Array : public MANAGEDOBJECT<Array<T>,Managed::ArrayData<T>>
    {
    public:

        

    };
}


#endif // EEN_COLLECTIONS_ARRAY_HPP