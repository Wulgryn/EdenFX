#ifndef EDEN_JSON_JSON_VALUE_HPP
#define EDEN_JSON_JSON_VALUE_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "text/string.hpp"

#include <memory>

namespace Eden::Text::Json
{
    enum class JsonValueType
    {
        Null,
        Boolean,
        Number,
        String,
        Array,
        Object
    };

    struct JsonInternal;

    namespace Managed
    {
        class JsonValueData : public virtual OBJECT
        {
        public:
            std::shared_ptr<JsonInternal> m_Internal;
        };
    }

    class JsonValue : public MANAGEDOBJECT<JsonValue,Managed::JsonValueData>
    {
    public:
        JsonValue();
        JsonValue(std::nullptr_t);
        JsonValue(bool value);
        JsonValue(int value);
        JsonValue(long long value);
        JsonValue(float value);
        JsonValue(double value);
        JsonValue(const char* value);
        JsonValue(const Eden::Text::String& value);

        bool is_null() const;
        bool is_boolean() const;
        bool is_number() const;
        bool is_string() const;
        bool is_array() const;
        bool is_object() const;

        JsonValueType type() const;

        bool get_bool(bool defaultValue = false) const;
        double get_number(double defaultValue = 0.0) const;
        int get_int(int defaultValue = 0) const;
        long long get_int64(long long defaultValue = 0) const;
        Eden::Text::String get_string(const Eden::Text::String& defaultValue = "") const;

        void set_null();
        void set_bool(bool value);
        void set_number(double value);
        void set_string(const Eden::Text::String& value);
        void set_array();
        void set_object();

        int count() const;
        void clear();

        JsonValue at(int index) const;
        JsonValue operator[](int index) const;
        void add(const JsonValue& value);
        void add(const Eden::Text::String& value);
        void add(const char* value);
        void add(double value);
        void add(bool value);
        bool remove_at(int index);

        bool contains(const Eden::Text::String& name) const;
        JsonValue get(const Eden::Text::String& name) const;
        JsonValue operator[](const Eden::Text::String& name) const;
        void set(const Eden::Text::String& name, const JsonValue& value);
        void set(const Eden::Text::String& name, const Eden::Text::String& value);
        void set(const Eden::Text::String& name, const char* value);
        void set(const Eden::Text::String& name, double value);
        void set(const Eden::Text::String& name, bool value);
        bool remove(const Eden::Text::String& name);

        Collections::List<Eden::Text::String> property_names() const;
        Collections::List<JsonValue> values() const;

        Eden::Text::String to_json(bool indented = false) const;
        Eden::Text::String to_string() const;

        static JsonValue Null();
        static JsonValue Array();
        static JsonValue Object();
        static JsonValue Parse(const Eden::Text::String& json);

    private:
        explicit JsonValue(const std::shared_ptr<JsonInternal>& internal);

        friend class JsonDocument;
        friend class JsonParser;
    };

    using JsonElement = JsonValue;
    using JsonNode = JsonValue;
}

#if defined(AUTO_USINGS)
using namespace Eden::Text::Json;
#endif

#endif // EDEN_JSON_JSON_VALUE_HPP
