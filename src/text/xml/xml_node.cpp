#include "text/xml/xml_node.hpp"

#include <algorithm>
#include <sstream>
#include <vector>

namespace Eden::Text::Xml
{
    struct XmlNodeInternal
    {
        XmlNodeType m_Type = XmlNodeType::None;
        std::string m_Name;
        std::string m_Value;
        std::vector<std::pair<std::string, std::string>> m_Attributes;
        std::vector<std::shared_ptr<XmlNodeInternal>> m_Children;
        std::weak_ptr<XmlNodeInternal> m_Parent;
    };

    static std::string EscapeXml(const std::string& value)
    {
        std::string result;

        for (char c : value)
        {
            switch (c)
            {
                case '&': result += "&amp;"; break;
                case '<': result += "&lt;"; break;
                case '>': result += "&gt;"; break;
                case '"': result += "&quot;"; break;
                case '\'': result += "&apos;"; break;
                case '\r': result += "&#xD;"; break;
                case '\n': result += "&#xA;"; break;
                default: result.push_back(c); break;
            }
        }

        return result;
    }

    static void CollectInnerText(const std::shared_ptr<XmlNodeInternal>& node, std::string& result)
    {
        if (!node)
        {
            return;
        }

        if (node->m_Type == XmlNodeType::Text)
        {
            result += node->m_Value;
        }

        for (const auto& child : node->m_Children)
        {
            CollectInnerText(child, result);
        }
    }

    static void SerializeNode(const std::shared_ptr<XmlNodeInternal>& node, std::ostringstream& builder, bool indented, int depth)
    {
        if (!node)
        {
            return;
        }

        auto writeIndent = [&]()
        {
            if (indented)
            {
                for (int i = 0; i < depth; i++)
                {
                    builder << "  ";
                }
            }
        };

        if (node->m_Type == XmlNodeType::Text)
        {
            builder << EscapeXml(node->m_Value);
            return;
        }

        if (node->m_Type == XmlNodeType::Comment)
        {
            writeIndent();
            builder << "<!--" << node->m_Value << "-->";
            return;
        }

        if (node->m_Type == XmlNodeType::Declaration)
        {
            writeIndent();
            builder << "<?" << node->m_Value << "?>";
            return;
        }

        if (node->m_Type != XmlNodeType::Element)
        {
            return;
        }

        writeIndent();
        builder << '<' << node->m_Name;

        for (const auto& attribute : node->m_Attributes)
        {
            builder << ' ' << attribute.first << "=\"" << EscapeXml(attribute.second) << "\"";
        }

        if (node->m_Children.empty())
        {
            builder << " />";
            return;
        }

        bool onlyTextChild = node->m_Children.size() == 1 && node->m_Children[0]->m_Type == XmlNodeType::Text;

        builder << '>';

        if (indented && !onlyTextChild)
        {
            builder << '\n';
        }

        for (std::size_t i = 0; i < node->m_Children.size(); i++)
        {
            SerializeNode(node->m_Children[i], builder, indented, depth + 1);

            if (indented && !onlyTextChild && i + 1 < node->m_Children.size())
            {
                builder << '\n';
            }
        }

        if (indented && !onlyTextChild)
        {
            builder << '\n';
            writeIndent();
        }

        builder << "</" << node->m_Name << '>';
    }

    static Collections::List<Eden::Text::String> SplitPath(const Eden::Text::String& path)
    {
        return path.replace('\\', '/').split('/', true);
    }

    XmlAttribute::XmlAttribute() = default;

    XmlAttribute::XmlAttribute(const Eden::Text::String& name, const Eden::Text::String& value)
    {
        this->data()->m_Name = name;
        this->data()->m_Value = value;
    }

    Eden::Text::String XmlAttribute::name() const
    {
        return this->data()->m_Name;
    }

    Eden::Text::String XmlAttribute::value() const
    {
        return this->data()->m_Value;
    }

    void XmlAttribute::set_name(const Eden::Text::String& name)
    {
        this->data()->m_Name = name;
    }

    void XmlAttribute::set_value(const Eden::Text::String& value)
    {
        this->data()->m_Value = value;
    }

    XmlNode::XmlNode() = default;

    XmlNode::XmlNode(const std::shared_ptr<XmlNodeInternal>& internal)
    {
        this->data()->m_Internal = internal;
    }

    bool XmlNode::is_null() const
    {
        return this->data()->m_Internal == nullptr;
    }

    XmlNodeType XmlNode::node_type() const
    {
        return is_null() ? XmlNodeType::None : this->data()->m_Internal->m_Type;
    }

    Eden::Text::String XmlNode::name() const
    {
        return is_null() ? Eden::Text::String() : Eden::Text::String(this->data()->m_Internal->m_Name);
    }

    Eden::Text::String XmlNode::value() const
    {
        return is_null() ? Eden::Text::String() : Eden::Text::String(this->data()->m_Internal->m_Value);
    }

    Eden::Text::String XmlNode::inner_text() const
    {
        std::string result;
        CollectInnerText(this->data()->m_Internal, result);
        return Eden::Text::String(result);
    }

    Eden::Text::String XmlNode::inner_xml(bool indented) const
    {
        if (is_null())
        {
            return Eden::Text::String();
        }

        std::ostringstream builder;

        for (std::size_t i = 0; i < this->data()->m_Internal->m_Children.size(); i++)
        {
            SerializeNode(this->data()->m_Internal->m_Children[i], builder, indented, 0);

            if (indented && i + 1 < this->data()->m_Internal->m_Children.size())
            {
                builder << '\n';
            }
        }

        return Eden::Text::String(builder.str());
    }

    Eden::Text::String XmlNode::outer_xml(bool indented) const
    {
        std::ostringstream builder;
        SerializeNode(this->data()->m_Internal, builder, indented, 0);
        return Eden::Text::String(builder.str());
    }

    void XmlNode::set_name(const Eden::Text::String& name)
    {
        if (!is_null())
        {
            this->data()->m_Internal->m_Name = name.std();
        }
    }

    void XmlNode::set_value(const Eden::Text::String& value)
    {
        if (!is_null())
        {
            this->data()->m_Internal->m_Value = value.std();
        }
    }

    bool XmlNode::has_attribute(const Eden::Text::String& name) const
    {
        if (is_null())
        {
            return false;
        }

        const std::string key = name.std();

        for (const auto& attribute : this->data()->m_Internal->m_Attributes)
        {
            if (attribute.first == key)
            {
                return true;
            }
        }

        return false;
    }

    Eden::Text::String XmlNode::get_attribute(const Eden::Text::String& name) const
    {
        if (is_null())
        {
            return Eden::Text::String();
        }

        const std::string key = name.std();

        for (const auto& attribute : this->data()->m_Internal->m_Attributes)
        {
            if (attribute.first == key)
            {
                return Eden::Text::String(attribute.second.c_str());
            }
        }

        return Eden::Text::String();
    }

    void XmlNode::set_attribute(const Eden::Text::String& name, const Eden::Text::String& value)
    {
        if (is_null())
        {
            return;
        }

        const std::string key = name.std();

        for (auto& attribute : this->data()->m_Internal->m_Attributes)
        {
            if (attribute.first == key)
            {
                attribute.second = value.std();
                return;
            }
        }

        this->data()->m_Internal->m_Attributes.push_back({ key, value.std() });
    }

    bool XmlNode::remove_attribute(const Eden::Text::String& name)
    {
        if (is_null())
        {
            return false;
        }

        const std::string key = name.std();
        auto& attributes = this->data()->m_Internal->m_Attributes;

        auto it = std::remove_if(attributes.begin(), attributes.end(), [&](const auto& attribute)
        {
            return attribute.first == key;
        });

        if (it == attributes.end())
        {
            return false;
        }

        attributes.erase(it, attributes.end());
        return true;
    }

    Collections::List<XmlAttribute> XmlNode::attributes() const
    {
        Collections::List<XmlAttribute> result;

        if (is_null())
        {
            return result;
        }

        for (const auto& attribute : this->data()->m_Internal->m_Attributes)
        {
            result.add(XmlAttribute(Eden::Text::String(attribute.first.c_str()), Eden::Text::String(attribute.second.c_str())));
        }

        return result;
    }

    int XmlNode::child_count() const
    {
        return is_null() ? 0 : static_cast<int>(this->data()->m_Internal->m_Children.size());
    }

    Collections::List<XmlNode> XmlNode::child_nodes() const
    {
        Collections::List<XmlNode> result;

        if (is_null())
        {
            return result;
        }

        for (const auto& child : this->data()->m_Internal->m_Children)
        {
            result.add(XmlNode(child));
        }

        return result;
    }

    Collections::List<XmlNode> XmlNode::elements() const
    {
        Collections::List<XmlNode> result;

        if (is_null())
        {
            return result;
        }

        for (const auto& child : this->data()->m_Internal->m_Children)
        {
            if (child->m_Type == XmlNodeType::Element)
            {
                result.add(XmlNode(child));
            }
        }

        return result;
    }

    Collections::List<XmlNode> XmlNode::elements(const Eden::Text::String& name) const
    {
        Collections::List<XmlNode> result;

        if (is_null())
        {
            return result;
        }

        const std::string key = name.std();

        for (const auto& child : this->data()->m_Internal->m_Children)
        {
            if (child->m_Type == XmlNodeType::Element && child->m_Name == key)
            {
                result.add(XmlNode(child));
            }
        }

        return result;
    }

    XmlNode XmlNode::first_child() const
    {
        if (is_null() || this->data()->m_Internal->m_Children.empty())
        {
            return XmlNode();
        }

        return XmlNode(this->data()->m_Internal->m_Children.front());
    }

    XmlNode XmlNode::last_child() const
    {
        if (is_null() || this->data()->m_Internal->m_Children.empty())
        {
            return XmlNode();
        }

        return XmlNode(this->data()->m_Internal->m_Children.back());
    }

    XmlNode XmlNode::parent_node() const
    {
        if (is_null())
        {
            return XmlNode();
        }

        return XmlNode(this->data()->m_Internal->m_Parent.lock());
    }

    XmlNode XmlNode::append_child(const XmlNode& child)
    {
        if (is_null() || child.is_null())
        {
            return XmlNode();
        }

        child.data()->m_Internal->m_Parent = this->data()->m_Internal;
        this->data()->m_Internal->m_Children.push_back(child.data()->m_Internal);
        return child;
    }

    XmlNode XmlNode::prepend_child(const XmlNode& child)
    {
        if (is_null() || child.is_null())
        {
            return XmlNode();
        }

        child.data()->m_Internal->m_Parent = this->data()->m_Internal;
        this->data()->m_Internal->m_Children.insert(this->data()->m_Internal->m_Children.begin(), child.data()->m_Internal);
        return child;
    }

    bool XmlNode::remove_child(const XmlNode& child)
    {
        if (is_null() || child.is_null())
        {
            return false;
        }

        auto& children = this->data()->m_Internal->m_Children;
        auto it = std::find(children.begin(), children.end(), child.data()->m_Internal);

        if (it == children.end())
        {
            return false;
        }

        (*it)->m_Parent.reset();
        children.erase(it);
        return true;
    }

    void XmlNode::remove_all()
    {
        if (!is_null())
        {
            for (auto& child : this->data()->m_Internal->m_Children)
            {
                child->m_Parent.reset();
            }

            this->data()->m_Internal->m_Children.clear();
            this->data()->m_Internal->m_Attributes.clear();
        }
    }

    XmlNode XmlNode::select_single_node(const Eden::Text::String& path) const
    {
        auto nodes = select_nodes(path);
        return nodes.count() == 0 ? XmlNode() : nodes[0];
    }

    Collections::List<XmlNode> XmlNode::select_nodes(const Eden::Text::String& path) const
    {
        Collections::List<XmlNode> current;

        if (is_null())
        {
            return current;
        }

        current.add(*this);
        Collections::List<Eden::Text::String> parts = SplitPath(path);

        for (int i = 0; i < parts.count(); i++)
        {
            Collections::List<XmlNode> next;

            for (int n = 0; n < current.count(); n++)
            {
                Collections::List<XmlNode> matches = current[n].elements(parts[i]);

                for (int m = 0; m < matches.count(); m++)
                {
                    next.add(matches[m]);
                }
            }

            current = next;
        }

        return current;
    }

    bool XmlNode::operator==(const XmlNode& other) const
    {
        return this->data()->m_Internal == other.data()->m_Internal;
    }

    bool XmlNode::operator!=(const XmlNode& other) const
    {
        return !(*this == other);
    }

    XmlNode XmlNode::Element(const Eden::Text::String& name)
    {
        auto internal = std::make_shared<XmlNodeInternal>();
        internal->m_Type = XmlNodeType::Element;
        internal->m_Name = name.std();
        return XmlNode(internal);
    }

    XmlNode XmlNode::Text(const Eden::Text::String& text)
    {
        auto internal = std::make_shared<XmlNodeInternal>();
        internal->m_Type = XmlNodeType::Text;
        internal->m_Value = text.std();
        return XmlNode(internal);
    }

    XmlNode XmlNode::Comment(const Eden::Text::String& text)
    {
        auto internal = std::make_shared<XmlNodeInternal>();
        internal->m_Type = XmlNodeType::Comment;
        internal->m_Value = text.std();
        return XmlNode(internal);
    }

    XmlNode XmlNode::Declaration(const Eden::Text::String& value)
    {
        auto internal = std::make_shared<XmlNodeInternal>();
        internal->m_Type = XmlNodeType::Declaration;
        internal->m_Value = value.std();
        return XmlNode(internal);
    }
}
