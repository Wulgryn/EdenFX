#ifndef PANDORAEX_ASYNC_LIST_HPP
#define PANDORAEX_ASYNC_LIST_HPP

#include "uniqueiList.hpp"

namespace PandoraEX
{
    template <class Type>
    class AsyncList : public List<Type>
    {
        UniqueList<int> removeList;
        bool __clear = false;

    public:
        void removeAt(size_t index) override
        {
            removeList.add(index);
        }

        void remove(const Type &item) override
        {
            removeList.add(List<Type>::indexOf(item));
        }

        void clear() override
        {
            __clear = true;
        }

        void validateRemovals()
        {
            if (__clear)
                List<Type>::clear();
            else
                for (size_t i = 0; i < removeList.size(); i++)
                    List<Type>::removeAt(removeList[i]);
            __clear = false;
            removeList.clear();
        }
    };
}

#endif // PANDORAEX_ASYNC_LIST_HPP