#ifndef EDEN_XML_XML_NODE_HPP
#define EDEN_XML_XML_NODE_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "text/string.hpp"

#include <memory>
#include <string>

namespace Eden::Text::Xml
{
    enum class XmlNodeType
    {
        None,
        Element,
        Text,
        Comment,
        Declaration
    };

    struct XmlNodeInternal;

    namespace Managed
    {
        class XmlAttributeData : public virtual OBJECT
        {
        public:
            Eden::Text::String m_Name;
            Eden::Text::String m_Value;
        };

        class XmlNodeData : public virtual OBJECT
        {
        public:
            std::shared_ptr<XmlNodeInternal> m_Internal;
        };
    }

    class XmlAttribute : public MANAGEDOBJECT<XmlAttribute,Managed::XmlAttributeData>
    {
    public:
        XmlAttribute();
        XmlAttribute(const Eden::Text::String& name, const Eden::Text::String& value = "");

        Eden::Text::String name() const;
        Eden::Text::String value() const;

        void set_name(const Eden::Text::String& name);
        void set_value(const Eden::Text::String& value);
    };

    class XmlNode : public MANAGEDOBJECT<XmlNode,Managed::XmlNodeData>
    {
    public:
        XmlNode();

        bool is_null() const;
        XmlNodeType node_type() const;

        Eden::Text::String name() const;
        Eden::Text::String value() const;
        Eden::Text::String inner_text() const;
        Eden::Text::String inner_xml(bool indented = false) const;
        Eden::Text::String outer_xml(bool indented = false) const;

        void set_name(const Eden::Text::String& name);
        void set_value(const Eden::Text::String& value);

        bool has_attribute(const Eden::Text::String& name) const;
        Eden::Text::String get_attribute(const Eden::Text::String& name) const;
        void set_attribute(const Eden::Text::String& name, const Eden::Text::String& value);
        bool remove_attribute(const Eden::Text::String& name);
        Collections::List<XmlAttribute> attributes() const;

        int child_count() const;
        Collections::List<XmlNode> child_nodes() const;
        Collections::List<XmlNode> elements() const;
        Collections::List<XmlNode> elements(const Eden::Text::String& name) const;

        XmlNode first_child() const;
        XmlNode last_child() const;
        XmlNode parent_node() const;

        XmlNode append_child(const XmlNode& child);
        XmlNode prepend_child(const XmlNode& child);
        bool remove_child(const XmlNode& child);
        void remove_all();

        XmlNode select_single_node(const Eden::Text::String& path) const;
        Collections::List<XmlNode> select_nodes(const Eden::Text::String& path) const;

        bool operator==(const XmlNode& other) const;
        bool operator!=(const XmlNode& other) const;

        static XmlNode Element(const Eden::Text::String& name);
        static XmlNode Text(const Eden::Text::String& text);
        static XmlNode Comment(const Eden::Text::String& text);
        static XmlNode Declaration(const Eden::Text::String& value);

    private:
        explicit XmlNode(const std::shared_ptr<XmlNodeInternal>& internal);

        friend class XmlDocument;
        friend class XmlParser;
    };

    using XmlElement = XmlNode;
    using XmlText = XmlNode;
    using XmlComment = XmlNode;
    using XmlNodeList = Collections::List<XmlNode>;
    using XmlAttributeList = Collections::List<XmlAttribute>;
}

#if defined(AUTO_USINGS)
using namespace Eden::Text::Xml;
#endif

#endif // EDEN_XML_XML_NODE_HPP
