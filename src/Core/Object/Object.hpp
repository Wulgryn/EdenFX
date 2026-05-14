#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

namespace Eden::Core::Object
{
    class Object
    {
        public:
        virtual ~Object() = default;
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::Core::Object;
#endif

#define OBJECT Eden::Core::Object::Object 

#endif // CORE_OBJECT_HPP