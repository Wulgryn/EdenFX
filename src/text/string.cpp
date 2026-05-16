#include "text/string.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace Eden::Text
{
    String::String() = default;

    String::String(const char *value)
    {
        this->data()->m_Value = value == nullptr ? std::string() : std::string(value);
    }

    String::String(char value)
    {
        this->data()->m_Value = std::string(1, value);
    }

    String::String(std::string_view value)
    {
        this->data()->m_Value = std::string(value);
    }

    String::String(int count, char value)
    {
        if (count > 0)
        {
            this->data()->m_Value = std::string(static_cast<std::size_t>(count), value);
        }
    }

    const char *String::c_str() const
    {
        return this->data()->m_Value.c_str();
    }

    std::string String::std() const
    {
        return this->data()->m_Value;
    }

    std::string String::to_std_string() const
    {
        return this->data()->m_Value;
    }

    String String::to_string() const
    {
        return *this;
    }

    String::operator std::string() const
    {
        return this->data()->m_Value;
    }

    String::operator const char *() const
    {
        return this->data()->m_Value.c_str();
    }

    int String::length() const
    {
        return static_cast<int>(this->data()->m_Value.size());
    }

    bool String::empty() const
    {
        return this->data()->m_Value.empty();
    }

    void String::clear()
    {
        this->data()->m_Value.clear();
    }

    char &String::operator[](int index)
    {
        return this->data()->m_Value[static_cast<std::size_t>(index)];
    }

    const char &String::operator[](int index) const
    {
        return this->data()->m_Value[static_cast<std::size_t>(index)];
    }

    String String::operator+(const String &other) const
    {
        return String(this->data()->m_Value + other.std());
    }

    String &String::operator+=(const String &other)
    {
        this->data()->m_Value += other.std();
        return *this;
    }

    bool String::operator==(const String &other) const
    {
        return this->data()->m_Value == other.std();
    }

    bool String::operator!=(const String &other) const
    {
        return !(*this == other);
    }

    bool String::operator<(const String &other) const
    {
        return this->data()->m_Value < other.std();
    }

    bool String::operator>(const String &other) const
    {
        return this->data()->m_Value > other.std();
    }

    bool String::operator<=(const String &other) const
    {
        return this->data()->m_Value <= other.std();
    }

    bool String::operator>=(const String &other) const
    {
        return this->data()->m_Value >= other.std();
    }

    bool String::contains(const String &value) const
    {
        return this->data()->m_Value.find(value.std()) != std::string::npos;
    }

    bool String::starts_with(const String &value) const
    {
        const std::string &self = this->data()->m_Value;
        const std::string &other = value.std();

        if (other.size() > self.size())
        {
            return false;
        }

        return std::equal(other.begin(), other.end(), self.begin());
    }

    bool String::ends_with(const String &value) const
    {
        const std::string &self = this->data()->m_Value;
        const std::string &other = value.std();

        if (other.size() > self.size())
        {
            return false;
        }

        return std::equal(other.rbegin(), other.rend(), self.rbegin());
    }

    int String::index_of(const String &value) const
    {
        std::size_t index = this->data()->m_Value.find(value.std());
        return index == std::string::npos ? NPos : static_cast<int>(index);
    }

    int String::index_of(char value) const
    {
        std::size_t index = this->data()->m_Value.find(value);
        return index == std::string::npos ? NPos : static_cast<int>(index);
    }

    int String::last_index_of(const String &value) const
    {
        std::size_t index = this->data()->m_Value.rfind(value.std());
        return index == std::string::npos ? NPos : static_cast<int>(index);
    }

    int String::last_index_of(char value) const
    {
        std::size_t index = this->data()->m_Value.rfind(value);
        return index == std::string::npos ? NPos : static_cast<int>(index);
    }

    String String::substring(int startIndex) const
    {
        if (startIndex < 0)
        {
            startIndex = 0;
        }

        if (startIndex >= length())
        {
            return String();
        }

        return String(this->data()->m_Value.substr(static_cast<std::size_t>(startIndex)));
    }

    String String::substring(int startIndex, int length) const
    {
        if (startIndex < 0)
        {
            startIndex = 0;
        }

        if (length < 0 || startIndex >= this->length())
        {
            return String();
        }

        return String(this->data()->m_Value.substr(static_cast<std::size_t>(startIndex), static_cast<std::size_t>(length)));
    }

    String String::insert(int startIndex, const String &value) const
    {
        std::string result = this->data()->m_Value;

        if (startIndex < 0)
        {
            startIndex = 0;
        }

        if (startIndex > static_cast<int>(result.size()))
        {
            startIndex = static_cast<int>(result.size());
        }

        result.insert(static_cast<std::size_t>(startIndex), value.std());
        return String(result);
    }

    String String::remove(int startIndex) const
    {
        if (startIndex < 0)
        {
            startIndex = 0;
        }

        if (startIndex >= length())
        {
            return *this;
        }

        std::string result = this->data()->m_Value;
        result.erase(static_cast<std::size_t>(startIndex));
        return String(result);
    }

    String String::remove(int startIndex, int count) const
    {
        if (startIndex < 0)
        {
            startIndex = 0;
        }

        if (count < 0 || startIndex >= length())
        {
            return *this;
        }

        std::string result = this->data()->m_Value;
        result.erase(static_cast<std::size_t>(startIndex), static_cast<std::size_t>(count));
        return String(result);
    }

    String String::replace(const String &oldValue, const String &newValue) const
    {
        if (oldValue.empty())
        {
            return *this;
        }

        std::string result = this->data()->m_Value;
        std::size_t position = 0;

        while ((position = result.find(oldValue.std(), position)) != std::string::npos)
        {
            result.replace(position, oldValue.std().size(), newValue.std());
            position += newValue.std().size();
        }

        return String(result);
    }

    String String::replace(char oldValue, char newValue) const
    {
        std::string result = this->data()->m_Value;
        std::replace(result.begin(), result.end(), oldValue, newValue);
        return String(result);
    }

    String String::to_lower() const
    {
        std::string result = this->data()->m_Value;

        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                       { return static_cast<char>(std::tolower(c)); });

        return String(result);
    }

    String String::to_upper() const
    {
        std::string result = this->data()->m_Value;

        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                       { return static_cast<char>(std::toupper(c)); });

        return String(result);
    }

    String String::trim() const
    {
        return trim_start().trim_end();
    }

    String String::trim_start() const
    {
        const std::string &value = this->data()->m_Value;
        std::size_t start = 0;

        while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
        {
            start++;
        }

        return String(value.substr(start));
    }

    String String::trim_end() const
    {
        const std::string &value = this->data()->m_Value;

        if (value.empty())
        {
            return String();
        }

        std::size_t end = value.size();

        while (end > 0 && std::isspace(static_cast<unsigned char>(value[end - 1])))
        {
            end--;
        }

        return String(value.substr(0, end));
    }

    Collections::List<String> String::split(char separator, bool removeEmptyEntries) const
    {
        Collections::List<String> result;
        std::stringstream stream(this->data()->m_Value);
        std::string item;

        while (std::getline(stream, item, separator))
        {
            if (removeEmptyEntries && item.empty())
            {
                continue;
            }

            result.add(String(item));
        }

        if (!removeEmptyEntries && !this->data()->m_Value.empty() && this->data()->m_Value.back() == separator)
        {
            result.add(String());
        }

        return result;
    }

    bool String::IsNullOrEmpty(const String &value)
    {
        return value.empty();
    }

    bool String::IsNullOrWhiteSpace(const String &value)
    {
        for (char c : value.std())
        {
            if (!std::isspace(static_cast<unsigned char>(c)))
            {
                return false;
            }
        }

        return true;
    }

    String String::Join(const String &separator, const Collections::List<String> &values)
    {
        std::string result;

        for (int i = 0; i < values.count(); i++)
        {
            if (i > 0)
            {
                result += separator.std();
            }

            result += values[i].std();
        }

        return String(result);
    }

    String String::Concat(const Collections::List<String> &values)
    {
        std::string result;

        for (const String &value : values)
        {
            result += value.std();
        }

        return String(result);
    }

    String String::Empty()
    {
        return String();
    }

    std::string::iterator String::begin()
    {
        return this->data()->m_Value.begin();
    }

    std::string::const_iterator String::begin() const
    {
        return this->data()->m_Value.begin();
    }

    std::string::const_iterator String::cbegin() const
    {
        return this->data()->m_Value.cbegin();
    }

    std::string::iterator String::end()
    {
        return this->data()->m_Value.end();
    }

    std::string::const_iterator String::end() const
    {
        return this->data()->m_Value.end();
    }

    std::string::const_iterator String::cend() const
    {
        return this->data()->m_Value.cend();
    }
}
