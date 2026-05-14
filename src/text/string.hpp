#ifndef EDEN_TEXT_STRING_HPP
#define EDEN_TEXT_STRING_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"

#include <cstddef>
#include <initializer_list>
#include <string>
#include <string_view>

namespace Eden::Text
{
    namespace Managed
    {
        class StringData : public virtual OBJECT
        {
        public:
            std::string m_Value;
        };
    }

    class String : public MANAGEDOBJECT<Managed::StringData>
    {
    public:
        using NativeString = std::string;
        static constexpr int NPos = -1;

    public:
        String();
        String(const char* value);
        String(char value);
        String(std::string_view value);
        String(int count, char value);

        const std::string& native() const;
        std::string& native();

        const char* c_str() const;

        std::string std() const;
        std::string to_std_string() const;
        String to_string() const;

        operator std::string() const;
        explicit operator const char*() const;

        int length() const;
        bool empty() const;
        void clear();

        char& operator[](int index);
        const char& operator[](int index) const;

        String operator+(const String& other) const;
        String& operator+=(const String& other);

        bool operator==(const String& other) const;
        bool operator!=(const String& other) const;
        bool operator<(const String& other) const;
        bool operator>(const String& other) const;
        bool operator<=(const String& other) const;
        bool operator>=(const String& other) const;

        bool Contains(const String& value) const;
        bool contains(const String& value) const;

        bool starts_with(const String& value) const;

        bool ends_with(const String& value) const;

        int index_of(const String& value) const;
        int index_of(char value) const;
        int last_index_of(const String& value) const;
        int last_index_of(char value) const;

        String substring(int startIndex) const;
        String substring(int startIndex, int length) const;

        String insert(int startIndex, const String& value) const;
        String remove(int startIndex) const;
        String remove(int startIndex, int count) const;
        String replace(const String& oldValue, const String& newValue) const;
        String replace(char oldValue, char newValue) const;

        String to_lower() const;
        String to_upper() const;

        String trim() const;
        String trim_start() const;
        String trim_end() const;

        Collections::List<String> split(char separator, bool removeEmptyEntries = false) const;

        static bool IsNullOrEmpty(const String& value);
        static bool IsNullOrWhiteSpace(const String& value);
        static String Join(const String& separator, const Collections::List<String>& values);
        static String Concat(const Collections::List<String>& values);
        static String Empty();

        template <typename TValue>
        static String FromNumber(TValue value)
        {
            return String(std::to_string(value));
        }

        std::string::iterator begin();
        std::string::const_iterator begin() const;
        std::string::const_iterator cbegin() const;
        std::string::iterator end();
        std::string::const_iterator end() const;
        std::string::const_iterator cend() const;
    };
}

#endif // EDEN_TEXT_STRING_HPP
