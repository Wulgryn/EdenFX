#ifndef PANDORAEX_OBJECT_HPP
#define PANDORAEX_OBJECT_HPP

#include <string>

namespace std
{
    template <typename T>
    struct atomic;
}

namespace PandoraEX
{

    class Object
    {
        static std::atomic<unsigned long long> s_nextID;
        unsigned long long _id;

    public:
        std::string typeName = "Object";
        Object();
        // Object(std::string typeName);
        Object(const Object &other);
        // Object(Object &&other) noexcept;

        Object &operator=(const Object &other);
        // Object &operator=(Object &&other) noexcept;

        unsigned long long id() const;

        virtual ~Object();

        virtual std::string getTypeName() const;

        virtual bool isType(const std::string &_typeName) const;

        virtual bool isType(const Object &other) const;

        bool operator==(const Object &other) const;

        bool operator!=(const Object &other) const;

        bool equals(const Object &other) const;

        std::strong_ordering operator<=>(const Object &other) const;
    };
    template <typename Object>
    concept ObjectType = std::derived_from<std::remove_cvref_t<Object>, Object>;

/// @brief Macro to define a class that extends Object.
/// @param className The name of the class.
#define Class(...) class __VA_ARGS__ : public virtual PandoraEX::Object
/// @brief Macro to define a struct that extends Object.
/// @param structName The name of the struct.
#define Struct(...) struct __VA_ARGS__ : public virtual PandoraEX::Object
    // #define extends(...) , ##__VA_ARGS__

#define extends ,
#define pextends , public
#define public public

}

#endif // PANDORAEX_OBJECT_HPP