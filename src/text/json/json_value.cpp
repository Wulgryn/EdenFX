#include "text/json/json_value.hpp"

#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace Eden::Text::Json
{
    struct JsonInternal
    {
        JsonValueType m_Type = JsonValueType::Null;
        bool m_Bool = false;
        double m_Number = 0.0;
        std::string m_String;
        std::vector<std::shared_ptr<JsonInternal>> m_Array;
        std::vector<std::pair<std::string, std::shared_ptr<JsonInternal>>> m_Object;
    };

    static std::shared_ptr<JsonInternal> MakeInternal(JsonValueType type = JsonValueType::Null)
    {
        auto internal = std::make_shared<JsonInternal>();
        internal->m_Type = type;
        return internal;
    }

    static std::string EscapeJson(const std::string& value)
    {
        std::ostringstream builder;

        for (unsigned char c : value)
        {
            switch (c)
            {
                case '"': builder << "\\\""; break;
                case '\\': builder << "\\\\"; break;
                case '\b': builder << "\\b"; break;
                case '\f': builder << "\\f"; break;
                case '\n': builder << "\\n"; break;
                case '\r': builder << "\\r"; break;
                case '\t': builder << "\\t"; break;
                default:
                    if (c < 32)
                    {
                        builder << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                    }
                    else
                    {
                        builder << static_cast<char>(c);
                    }
                    break;
            }
        }

        return builder.str();
    }

    static void WriteIndent(std::ostringstream& builder, int depth)
    {
        for (int i = 0; i < depth; i++)
        {
            builder << "  ";
        }
    }

    static void SerializeJson(const std::shared_ptr<JsonInternal>& value, std::ostringstream& builder, bool indented, int depth)
    {
        if (!value || value->m_Type == JsonValueType::Null)
        {
            builder << "null";
            return;
        }

        switch (value->m_Type)
        {
            case JsonValueType::Boolean:
                builder << (value->m_Bool ? "true" : "false");
                break;

            case JsonValueType::Number:
                if (std::floor(value->m_Number) == value->m_Number)
                {
                    builder << static_cast<long long>(value->m_Number);
                }
                else
                {
                    builder << value->m_Number;
                }
                break;

            case JsonValueType::String:
                builder << '"' << EscapeJson(value->m_String) << '"';
                break;

            case JsonValueType::Array:
                builder << '[';

                if (indented && !value->m_Array.empty())
                {
                    builder << '\n';
                }

                for (std::size_t i = 0; i < value->m_Array.size(); i++)
                {
                    if (indented)
                    {
                        WriteIndent(builder, depth + 1);
                    }

                    SerializeJson(value->m_Array[i], builder, indented, depth + 1);

                    if (i + 1 < value->m_Array.size())
                    {
                        builder << ',';
                    }

                    if (indented)
                    {
                        builder << '\n';
                    }
                }

                if (indented && !value->m_Array.empty())
                {
                    WriteIndent(builder, depth);
                }

                builder << ']';
                break;

            case JsonValueType::Object:
                builder << '{';

                if (indented && !value->m_Object.empty())
                {
                    builder << '\n';
                }

                for (std::size_t i = 0; i < value->m_Object.size(); i++)
                {
                    if (indented)
                    {
                        WriteIndent(builder, depth + 1);
                    }

                    builder << '"' << EscapeJson(value->m_Object[i].first) << '"' << ':';

                    if (indented)
                    {
                        builder << ' ';
                    }

                    SerializeJson(value->m_Object[i].second, builder, indented, depth + 1);

                    if (i + 1 < value->m_Object.size())
                    {
                        builder << ',';
                    }

                    if (indented)
                    {
                        builder << '\n';
                    }
                }

                if (indented && !value->m_Object.empty())
                {
                    WriteIndent(builder, depth);
                }

                builder << '}';
                break;

            default:
                builder << "null";
                break;
        }
    }

    class JsonParser
    {
    private:
        std::string m_Text;
        std::size_t m_Index = 0;

    public:
        explicit JsonParser(const Eden::Text::String& json)
            : m_Text(json.std())
        {
        }

        std::shared_ptr<JsonInternal> parse()
        {
            skip_whitespace();
            auto result = parse_value();
            skip_whitespace();
            return result;
        }

    private:
        bool end() const
        {
            return m_Index >= m_Text.size();
        }

        char current() const
        {
            return end() ? '\0' : m_Text[m_Index];
        }

        char take()
        {
            return end() ? '\0' : m_Text[m_Index++];
        }

        bool starts_with(const std::string& value) const
        {
            return m_Text.compare(m_Index, value.size(), value) == 0;
        }

        void skip_whitespace()
        {
            while (!end() && std::isspace(static_cast<unsigned char>(current())))
            {
                m_Index++;
            }
        }

        void expect(char value)
        {
            if (take() != value)
            {
                throw std::runtime_error("Invalid JSON syntax.");
            }
        }

        std::shared_ptr<JsonInternal> parse_value()
        {
            skip_whitespace();

            if (end())
            {
                throw std::runtime_error("Unexpected end of JSON.");
            }

            if (current() == '"')
            {
                auto result = MakeInternal(JsonValueType::String);
                result->m_String = parse_string_raw();
                return result;
            }

            if (current() == '{')
            {
                return parse_object();
            }

            if (current() == '[')
            {
                return parse_array();
            }

            if (starts_with("true"))
            {
                m_Index += 4;
                auto result = MakeInternal(JsonValueType::Boolean);
                result->m_Bool = true;
                return result;
            }

            if (starts_with("false"))
            {
                m_Index += 5;
                auto result = MakeInternal(JsonValueType::Boolean);
                result->m_Bool = false;
                return result;
            }

            if (starts_with("null"))
            {
                m_Index += 4;
                return MakeInternal(JsonValueType::Null);
            }

            if (current() == '-' || std::isdigit(static_cast<unsigned char>(current())))
            {
                return parse_number();
            }

            throw std::runtime_error("Invalid JSON value.");
        }

        std::shared_ptr<JsonInternal> parse_object()
        {
            auto result = MakeInternal(JsonValueType::Object);
            expect('{');
            skip_whitespace();

            if (current() == '}')
            {
                m_Index++;
                return result;
            }

            while (!end())
            {
                skip_whitespace();
                std::string name = parse_string_raw();
                skip_whitespace();
                expect(':');
                auto value = parse_value();
                result->m_Object.push_back({ name, value });
                skip_whitespace();

                if (current() == '}')
                {
                    m_Index++;
                    return result;
                }

                expect(',');
            }

            throw std::runtime_error("Unclosed JSON object.");
        }

        std::shared_ptr<JsonInternal> parse_array()
        {
            auto result = MakeInternal(JsonValueType::Array);
            expect('[');
            skip_whitespace();

            if (current() == ']')
            {
                m_Index++;
                return result;
            }

            while (!end())
            {
                result->m_Array.push_back(parse_value());
                skip_whitespace();

                if (current() == ']')
                {
                    m_Index++;
                    return result;
                }

                expect(',');
            }

            throw std::runtime_error("Unclosed JSON array.");
        }

        std::shared_ptr<JsonInternal> parse_number()
        {
            std::size_t start = m_Index;

            if (current() == '-')
            {
                m_Index++;
            }

            while (!end() && std::isdigit(static_cast<unsigned char>(current())))
            {
                m_Index++;
            }

            if (!end() && current() == '.')
            {
                m_Index++;

                while (!end() && std::isdigit(static_cast<unsigned char>(current())))
                {
                    m_Index++;
                }
            }

            if (!end() && (current() == 'e' || current() == 'E'))
            {
                m_Index++;

                if (current() == '-' || current() == '+')
                {
                    m_Index++;
                }

                while (!end() && std::isdigit(static_cast<unsigned char>(current())))
                {
                    m_Index++;
                }
            }

            auto result = MakeInternal(JsonValueType::Number);
            result->m_Number = std::stod(m_Text.substr(start, m_Index - start));
            return result;
        }

        std::string parse_string_raw()
        {
            expect('"');
            std::string result;

            while (!end())
            {
                char c = take();

                if (c == '"')
                {
                    return result;
                }

                if (c == '\\')
                {
                    char escaped = take();

                    switch (escaped)
                    {
                        case '"': result.push_back('"'); break;
                        case '\\': result.push_back('\\'); break;
                        case '/': result.push_back('/'); break;
                        case 'b': result.push_back('\b'); break;
                        case 'f': result.push_back('\f'); break;
                        case 'n': result.push_back('\n'); break;
                        case 'r': result.push_back('\r'); break;
                        case 't': result.push_back('\t'); break;
                        case 'u':
                            // Egyszerű verzió: ASCII alatti \u00XX karaktereket kezeljük pontosan.
                            if (m_Index + 4 <= m_Text.size())
                            {
                                std::string hex = m_Text.substr(m_Index, 4);
                                m_Index += 4;
                                int code = std::stoi(hex, nullptr, 16);

                                if (code <= 0x7F)
                                {
                                    result.push_back(static_cast<char>(code));
                                }
                            }
                            break;
                        default:
                            result.push_back(escaped);
                            break;
                    }
                }
                else
                {
                    result.push_back(c);
                }
            }

            throw std::runtime_error("Unclosed JSON string.");
        }
    };

    JsonValue::JsonValue()
    {
        this->data()->m_Internal = MakeInternal(JsonValueType::Null);
    }

    JsonValue::JsonValue(std::nullptr_t)
        : JsonValue()
    {
    }

    JsonValue::JsonValue(bool value)
    {
        set_bool(value);
    }

    JsonValue::JsonValue(int value)
    {
        set_number(static_cast<double>(value));
    }

    JsonValue::JsonValue(long long value)
    {
        set_number(static_cast<double>(value));
    }

    JsonValue::JsonValue(float value)
    {
        set_number(static_cast<double>(value));
    }

    JsonValue::JsonValue(double value)
    {
        set_number(value);
    }

    JsonValue::JsonValue(const char* value)
    {
        set_string(value == nullptr ? Eden::Text::String() : Eden::Text::String(value));
    }

    JsonValue::JsonValue(const Eden::Text::String& value)
    {
        set_string(value);
    }

    JsonValue::JsonValue(const std::shared_ptr<JsonInternal>& internal)
    {
        this->data()->m_Internal = internal == nullptr ? MakeInternal(JsonValueType::Null) : internal;
    }

    bool JsonValue::is_null() const
    {
        return !this->data()->m_Internal || this->data()->m_Internal->m_Type == JsonValueType::Null;
    }

    bool JsonValue::is_boolean() const
    {
        return type() == JsonValueType::Boolean;
    }

    bool JsonValue::is_number() const
    {
        return type() == JsonValueType::Number;
    }

    bool JsonValue::is_string() const
    {
        return type() == JsonValueType::String;
    }

    bool JsonValue::is_array() const
    {
        return type() == JsonValueType::Array;
    }

    bool JsonValue::is_object() const
    {
        return type() == JsonValueType::Object;
    }

    JsonValueType JsonValue::type() const
    {
        return this->data()->m_Internal == nullptr ? JsonValueType::Null : this->data()->m_Internal->m_Type;
    }

    bool JsonValue::get_bool(bool defaultValue) const
    {
        return is_boolean() ? this->data()->m_Internal->m_Bool : defaultValue;
    }

    double JsonValue::get_number(double defaultValue) const
    {
        return is_number() ? this->data()->m_Internal->m_Number : defaultValue;
    }

    int JsonValue::get_int(int defaultValue) const
    {
        return is_number() ? static_cast<int>(this->data()->m_Internal->m_Number) : defaultValue;
    }

    long long JsonValue::get_int64(long long defaultValue) const
    {
        return is_number() ? static_cast<long long>(this->data()->m_Internal->m_Number) : defaultValue;
    }

    Eden::Text::String JsonValue::get_string(const Eden::Text::String& defaultValue) const
    {
        return is_string() ? Eden::Text::String(this->data()->m_Internal->m_String) : defaultValue;
    }

    void JsonValue::set_null()
    {
        this->data()->m_Internal = MakeInternal(JsonValueType::Null);
    }

    void JsonValue::set_bool(bool value)
    {
        this->data()->m_Internal = MakeInternal(JsonValueType::Boolean);
        this->data()->m_Internal->m_Bool = value;
    }

    void JsonValue::set_number(double value)
    {
        this->data()->m_Internal = MakeInternal(JsonValueType::Number);
        this->data()->m_Internal->m_Number = value;
    }

    void JsonValue::set_string(const Eden::Text::String& value)
    {
        this->data()->m_Internal = MakeInternal(JsonValueType::String);
        this->data()->m_Internal->m_String = value.std();
    }

    void JsonValue::set_array()
    {
        this->data()->m_Internal = MakeInternal(JsonValueType::Array);
    }

    void JsonValue::set_object()
    {
        this->data()->m_Internal = MakeInternal(JsonValueType::Object);
    }

    int JsonValue::count() const
    {
        if (is_array())
        {
            return static_cast<int>(this->data()->m_Internal->m_Array.size());
        }

        if (is_object())
        {
            return static_cast<int>(this->data()->m_Internal->m_Object.size());
        }

        return 0;
    }

    void JsonValue::clear()
    {
        if (is_array())
        {
            this->data()->m_Internal->m_Array.clear();
        }
        else if (is_object())
        {
            this->data()->m_Internal->m_Object.clear();
        }
        else
        {
            set_null();
        }
    }

    JsonValue JsonValue::at(int index) const
    {
        if (!is_array() || index < 0 || index >= count())
        {
            return JsonValue();
        }

        return JsonValue(this->data()->m_Internal->m_Array[static_cast<std::size_t>(index)]);
    }

    JsonValue JsonValue::operator[](int index) const
    {
        return at(index);
    }

    void JsonValue::add(const JsonValue& value)
    {
        if (!is_array())
        {
            set_array();
        }

        this->data()->m_Internal->m_Array.push_back(value.data()->m_Internal);
    }

    void JsonValue::add(const Eden::Text::String& value)
    {
        add(JsonValue(value));
    }

    void JsonValue::add(const char* value)
    {
        add(JsonValue(value));
    }

    void JsonValue::add(double value)
    {
        add(JsonValue(value));
    }

    void JsonValue::add(bool value)
    {
        add(JsonValue(value));
    }

    bool JsonValue::remove_at(int index)
    {
        if (!is_array() || index < 0 || index >= count())
        {
            return false;
        }

        this->data()->m_Internal->m_Array.erase(this->data()->m_Internal->m_Array.begin() + index);
        return true;
    }

    bool JsonValue::contains(const Eden::Text::String& name) const
    {
        if (!is_object())
        {
            return false;
        }

        const std::string key = name.std();

        for (const auto& property : this->data()->m_Internal->m_Object)
        {
            if (property.first == key)
            {
                return true;
            }
        }

        return false;
    }

    JsonValue JsonValue::get(const Eden::Text::String& name) const
    {
        if (!is_object())
        {
            return JsonValue();
        }

        const std::string key = name.std();

        for (const auto& property : this->data()->m_Internal->m_Object)
        {
            if (property.first == key)
            {
                return JsonValue(property.second);
            }
        }

        return JsonValue();
    }

    JsonValue JsonValue::operator[](const Eden::Text::String& name) const
    {
        return get(name);
    }

    void JsonValue::set(const Eden::Text::String& name, const JsonValue& value)
    {
        if (!is_object())
        {
            set_object();
        }

        const std::string key = name.std();

        for (auto& property : this->data()->m_Internal->m_Object)
        {
            if (property.first == key)
            {
                property.second = value.data()->m_Internal;
                return;
            }
        }

        this->data()->m_Internal->m_Object.push_back({ key, value.data()->m_Internal });
    }

    void JsonValue::set(const Eden::Text::String& name, const Eden::Text::String& value)
    {
        set(name, JsonValue(value));
    }

    void JsonValue::set(const Eden::Text::String& name, const char* value)
    {
        set(name, JsonValue(value));
    }

    void JsonValue::set(const Eden::Text::String& name, double value)
    {
        set(name, JsonValue(value));
    }

    void JsonValue::set(const Eden::Text::String& name, bool value)
    {
        set(name, JsonValue(value));
    }

    bool JsonValue::remove(const Eden::Text::String& name)
    {
        if (!is_object())
        {
            return false;
        }

        const std::string key = name.std();
        auto& object = this->data()->m_Internal->m_Object;

        for (auto it = object.begin(); it != object.end(); ++it)
        {
            if (it->first == key)
            {
                object.erase(it);
                return true;
            }
        }

        return false;
    }

    Collections::List<Eden::Text::String> JsonValue::property_names() const
    {
        Collections::List<Eden::Text::String> result;

        if (!is_object())
        {
            return result;
        }

        for (const auto& property : this->data()->m_Internal->m_Object)
        {
            result.add(Eden::Text::String(property.first.c_str()));
        }

        return result;
    }

    Collections::List<JsonValue> JsonValue::values() const
    {
        Collections::List<JsonValue> result;

        if (is_array())
        {
            for (const auto& item : this->data()->m_Internal->m_Array)
            {
                result.add(JsonValue(item));
            }
        }
        else if (is_object())
        {
            for (const auto& property : this->data()->m_Internal->m_Object)
            {
                result.add(JsonValue(property.second));
            }
        }

        return result;
    }

    Eden::Text::String JsonValue::to_json(bool indented) const
    {
        std::ostringstream builder;
        SerializeJson(this->data()->m_Internal, builder, indented, 0);
        return Eden::Text::String(builder.str());
    }

    Eden::Text::String JsonValue::to_string() const
    {
        if (is_string())
        {
            return get_string();
        }

        return to_json(false);
    }

    JsonValue JsonValue::Null()
    {
        return JsonValue();
    }

    JsonValue JsonValue::Array()
    {
        JsonValue result;
        result.set_array();
        return result;
    }

    JsonValue JsonValue::Object()
    {
        JsonValue result;
        result.set_object();
        return result;
    }

    JsonValue JsonValue::Parse(const Eden::Text::String& json)
    {
        JsonParser parser(json);
        return JsonValue(parser.parse());
    }
}
