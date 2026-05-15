#include "text/json/json_document.hpp"
#include "io/file.hpp"

namespace Eden::Text::Json
{
    JsonDocument::JsonDocument() = default;

    JsonDocument::JsonDocument(const JsonValue& root)
    {
        this->data()->m_Root = root;
    }

    JsonValue JsonDocument::root_element() const
    {
        return this->data()->m_Root;
    }

    JsonValue JsonDocument::root() const
    {
        return root_element();
    }

    void JsonDocument::set_root(const JsonValue& root)
    {
        this->data()->m_Root = root;
    }

    void JsonDocument::load(const Eden::IO::Path& path)
    {
        load_json(Eden::IO::File::ReadAllText(path));
    }

    void JsonDocument::load_json(const Eden::Text::String& json)
    {
        this->data()->m_Root = JsonValue::Parse(json);
    }

    void JsonDocument::save(const Eden::IO::Path& path, bool indented) const
    {
        Eden::IO::File::WriteAllText(path, to_json(indented));
    }

    Eden::Text::String JsonDocument::to_json(bool indented) const
    {
        return this->data()->m_Root.to_json(indented);
    }

    JsonDocument JsonDocument::Parse(const Eden::Text::String& json)
    {
        JsonDocument document;
        document.load_json(json);
        return document;
    }

    JsonDocument JsonDocument::Load(const Eden::IO::Path& path)
    {
        JsonDocument document;
        document.load(path);
        return document;
    }

    JsonDocument JsonDocument::CreateObject()
    {
        return JsonDocument(JsonValue::Object());
    }

    JsonDocument JsonDocument::CreateArray()
    {
        return JsonDocument(JsonValue::Array());
    }
}
