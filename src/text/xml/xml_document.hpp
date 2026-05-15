#ifndef EDEN_XML_XML_DOCUMENT_HPP
#define EDEN_XML_XML_DOCUMENT_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "io/path.hpp"
#include "text/string.hpp"
#include "text/xml/xml_node.hpp"

namespace Eden::Text::Xml
{
    namespace Managed
    {
        class XmlDocumentData : public virtual OBJECT
        {
        public:
            XmlNode m_DocumentElement;
            Eden::Text::String m_Declaration = "xml version=\"1.0\" encoding=\"utf-8\"";
        };
    }

    class XmlDocument : public MANAGEDOBJECT<XmlDocument,Managed::XmlDocumentData>
    {
    public:
        XmlDocument();

        XmlNode document_element() const;
        XmlNode root() const;
        Eden::Text::String declaration() const;

        void set_declaration(const Eden::Text::String& declaration);
        void set_document_element(const XmlNode& node);

        XmlNode create_element(const Eden::Text::String& name) const;
        XmlNode create_text_node(const Eden::Text::String& text) const;
        XmlNode create_comment(const Eden::Text::String& text) const;
        XmlAttribute create_attribute(const Eden::Text::String& name, const Eden::Text::String& value = "") const;

        XmlNode append_child(const XmlNode& node);

        void load(const Eden::IO::Path& path);
        void load_xml(const Eden::Text::String& xml);
        void save(const Eden::IO::Path& path, bool indented = true) const;

        Eden::Text::String inner_text() const;
        Eden::Text::String outer_xml(bool indented = true, bool includeDeclaration = true) const;

        XmlNode select_single_node(const Eden::Text::String& path) const;
        Collections::List<XmlNode> select_nodes(const Eden::Text::String& path) const;

        static XmlDocument Load(const Eden::IO::Path& path);
        static XmlDocument Parse(const Eden::Text::String& xml);
        static XmlDocument Create(const Eden::Text::String& rootName);
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::Text::Xml;
#endif

#endif // EDEN_XML_XML_DOCUMENT_HPP
