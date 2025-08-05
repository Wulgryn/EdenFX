#include "object.hpp"
#include <atomic>

using namespace PandoraEX;
using namespace std;

atomic<unsigned long long> Object::s_nextID{1};

Object::Object() : _id(s_nextID++) {}
// Object::Object(string typeName) : _id(s_nextID++), _typeName(std::move(typeName)) {}
Object::Object(const Object &other) : _id(other._id) {}
// Object::Object(Object &&other) noexcept : _id(other._id), typeName(std::move(other.typeName)) {
//     other._id = 0; // Reset the ID of the moved-from object.
// }
Object &Object::operator=(const Object &other) {
    if (this != &other) {
        _id = other._id;
        typeName = other.typeName;
    }
    return *this;
}
// Object &Object::operator=(Object &&other) noexcept {
//     if (this != &other) {
//         _id = other._id;
//         typeName = std::move(other.typeName);
//         other._id = 0; // Reset the ID of the moved-from object.
//     }
//     return *this;
// }
unsigned long long Object::id() const {
    return _id;
}
Object::~Object() = default;

string Object::getTypeName() const {
    return typeName;
}

bool Object::isType(const string &_typeName) const {
    return getTypeName() == _typeName;
}

bool Object::isType(const Object &other) const {
    return isType(other.getTypeName());
}

bool Object::operator==(const Object &other) const {
    return _id == other._id;
}

bool Object::operator!=(const Object &other) const {
    return !(*this == other);
}

bool Object::equals(const Object &other) const {
    return *this == other;
}

std::strong_ordering Object::operator<=>(const Object &other) const {
    return _id <=> other._id;
}
