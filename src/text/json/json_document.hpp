#ifndef EDEN_JSON_JSON_DOCUMENT_HPP
#define EDEN_JSON_JSON_DOCUMENT_HPP

#include "core/object/managed_object.hpp"
#include "io/path.hpp"
#include "text/json/json_value.hpp"
#include "text/string.hpp"

namespace Eden::Text::Json
{
    namespace Managed
    {
        class JsonDocumentData : public virtual OBJECT
        {
        public:
            JsonValue m_Root;
        };
    }

    class JsonDocument : public MANAGEDOBJECT<JsonDocument,Managed::JsonDocumentData>
    {
    public:
        JsonDocument();
        explicit JsonDocument(const JsonValue& root);

        JsonValue root_element() const;
        JsonValue root() const;
        void set_root(const JsonValue& root);

        void load(const Eden::IO::Path& path);
        void load_json(const Eden::Text::String& json);
        void save(const Eden::IO::Path& path, bool indented = true) const;

        Eden::Text::String to_json(bool indented = true) const;

        static JsonDocument Parse(const Eden::Text::String& json);
        static JsonDocument Load(const Eden::IO::Path& path);
        static JsonDocument CreateObject();
        static JsonDocument CreateArray();
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::Text::Json;
#endif

#endif // EDEN_JSON_JSON_DOCUMENT_HPP
