#include "text/xml/xml_document.hpp"
#include "io/file.hpp"

#include <cctype>
#include <stdexcept>
#include <string>

namespace Eden::Text::Xml
{
    static std::string UnescapeXml(const std::string& value)
    {
        std::string result;

        for (std::size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == '&')
            {
                if (value.compare(i, 5, "&amp;") == 0)
                {
                    result.push_back('&');
                    i += 4;
                }
                else if (value.compare(i, 4, "&lt;") == 0)
                {
                    result.push_back('<');
                    i += 3;
                }
                else if (value.compare(i, 4, "&gt;") == 0)
                {
                    result.push_back('>');
                    i += 3;
                }
                else if (value.compare(i, 6, "&quot;") == 0)
                {
                    result.push_back('"');
                    i += 5;
                }
                else if (value.compare(i, 6, "&apos;") == 0)
                {
                    result.push_back('\'');
                    i += 5;
                }
                else
                {
                    result.push_back(value[i]);
                }
            }
            else
            {
                result.push_back(value[i]);
            }
        }

        return result;
    }

    class XmlParser
    {
    private:
        std::string m_Text;
        std::size_t m_Index = 0;

    public:
        explicit XmlParser(const Eden::Text::String& text)
            : m_Text(text.std())
        {
        }

        XmlNode parse_document(Eden::Text::String& declaration)
        {
            skip_whitespace();

            if (starts_with("<?"))
            {
                m_Index += 2;
                std::size_t end = m_Text.find("?>", m_Index);

                if (end == std::string::npos)
                {
                    throw std::runtime_error("Invalid XML declaration.");
                }

                declaration = Eden::Text::String(m_Text.substr(m_Index, end - m_Index).c_str());
                m_Index = end + 2;
            }

            while (true)
            {
                skip_whitespace();

                if (starts_with("<!--"))
                {
                    parse_comment();
                    continue;
                }

                break;
            }

            skip_whitespace();
            return parse_node();
        }

    private:
        bool end() const
        {
            return m_Index >= m_Text.size();
        }

        bool starts_with(const std::string& value) const
        {
            return m_Text.compare(m_Index, value.size(), value) == 0;
        }

        char current() const
        {
            return end() ? '\0' : m_Text[m_Index];
        }

        char take()
        {
            return end() ? '\0' : m_Text[m_Index++];
        }

        void expect(char value)
        {
            if (take() != value)
            {
                throw std::runtime_error("Invalid XML syntax.");
            }
        }

        void skip_whitespace()
        {
            while (!end() && std::isspace(static_cast<unsigned char>(current())))
            {
                m_Index++;
            }
        }

        std::string parse_name()
        {
            std::size_t start = m_Index;

            while (!end())
            {
                char c = current();

                if (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-' || c == ':' || c == '.')
                {
                    m_Index++;
                }
                else
                {
                    break;
                }
            }

            if (start == m_Index)
            {
                throw std::runtime_error("XML name expected.");
            }

            return m_Text.substr(start, m_Index - start);
        }

        XmlNode parse_node()
        {
            if (starts_with("<!--"))
            {
                return parse_comment();
            }

            if (current() == '<')
            {
                return parse_element();
            }

            return parse_text();
        }

        XmlNode parse_comment()
        {
            if (!starts_with("<!--"))
            {
                throw std::runtime_error("XML comment expected.");
            }

            m_Index += 4;
            std::size_t end = m_Text.find("-->", m_Index);

            if (end == std::string::npos)
            {
                throw std::runtime_error("Unclosed XML comment.");
            }

            std::string value = m_Text.substr(m_Index, end - m_Index);
            m_Index = end + 3;
            return XmlNode::Comment(value.c_str());
        }

        XmlNode parse_text()
        {
            std::size_t start = m_Index;

            while (!end() && current() != '<')
            {
                m_Index++;
            }

            return XmlNode::Text(UnescapeXml(m_Text.substr(start, m_Index - start)).c_str());
        }

        XmlNode parse_element()
        {
            expect('<');
            std::string name = parse_name();
            XmlNode element = XmlNode::Element(name.c_str());

            while (!end())
            {
                skip_whitespace();

                if (starts_with("/>"))
                {
                    m_Index += 2;
                    return element;
                }

                if (current() == '>')
                {
                    m_Index++;
                    break;
                }

                std::string attributeName = parse_name();
                skip_whitespace();
                expect('=');
                skip_whitespace();

                char quote = take();

                if (quote != '"' && quote != '\'')
                {
                    throw std::runtime_error("XML attribute quote expected.");
                }

                std::size_t valueStart = m_Index;

                while (!end() && current() != quote)
                {
                    m_Index++;
                }

                if (end())
                {
                    throw std::runtime_error("Unclosed XML attribute.");
                }

                std::string attributeValue = m_Text.substr(valueStart, m_Index - valueStart);
                expect(quote);
                element.set_attribute(attributeName.c_str(), UnescapeXml(attributeValue).c_str());
            }

            while (!end())
            {
                if (starts_with("</"))
                {
                    m_Index += 2;
                    std::string closeName = parse_name();
                    skip_whitespace();
                    expect('>');

                    if (closeName != name)
                    {
                        throw std::runtime_error("XML closing tag does not match opening tag.");
                    }

                    return element;
                }

                XmlNode child = parse_node();

                if (!child.is_null())
                {
                    element.append_child(child);
                }
            }

            throw std::runtime_error("Unclosed XML element.");
        }
    };

    XmlDocument::XmlDocument() = default;

    XmlNode XmlDocument::document_element() const
    {
        return this->data()->m_DocumentElement;
    }

    XmlNode XmlDocument::root() const
    {
        return document_element();
    }

    Eden::Text::String XmlDocument::declaration() const
    {
        return this->data()->m_Declaration;
    }

    void XmlDocument::set_declaration(const Eden::Text::String& declaration)
    {
        this->data()->m_Declaration = declaration;
    }

    void XmlDocument::set_document_element(const XmlNode& node)
    {
        this->data()->m_DocumentElement = node;
    }

    XmlNode XmlDocument::create_element(const Eden::Text::String& name) const
    {
        return XmlNode::Element(name);
    }

    XmlNode XmlDocument::create_text_node(const Eden::Text::String& text) const
    {
        return XmlNode::Text(text);
    }

    XmlNode XmlDocument::create_comment(const Eden::Text::String& text) const
    {
        return XmlNode::Comment(text);
    }

    XmlAttribute XmlDocument::create_attribute(const Eden::Text::String& name, const Eden::Text::String& value) const
    {
        return XmlAttribute(name, value);
    }

    XmlNode XmlDocument::append_child(const XmlNode& node)
    {
        this->data()->m_DocumentElement = node;
        return node;
    }

    void XmlDocument::load(const Eden::IO::Path& path)
    {
        load_xml(Eden::IO::File::ReadAllText(path));
    }

    void XmlDocument::load_xml(const Eden::Text::String& xml)
    {
        XmlParser parser(xml);
        Eden::Text::String declaration = this->data()->m_Declaration;
        this->data()->m_DocumentElement = parser.parse_document(declaration);
        this->data()->m_Declaration = declaration;
    }

    void XmlDocument::save(const Eden::IO::Path& path, bool indented) const
    {
        Eden::IO::File::WriteAllText(path, outer_xml(indented, true));
    }

    Eden::Text::String XmlDocument::inner_text() const
    {
        return this->data()->m_DocumentElement.inner_text();
    }

    Eden::Text::String XmlDocument::outer_xml(bool indented, bool includeDeclaration) const
    {
        Eden::Text::String result;

        if (includeDeclaration && !this->data()->m_Declaration.empty())
        {
            result += Eden::Text::String("<?");
            result += this->data()->m_Declaration;
            result += Eden::Text::String("?>");

            if (indented)
            {
                result += Eden::Text::String("\n");
            }
        }

        result += this->data()->m_DocumentElement.outer_xml(indented);
        return result;
    }

    XmlNode XmlDocument::select_single_node(const Eden::Text::String& path) const
    {
        if (this->data()->m_DocumentElement.is_null())
        {
            return XmlNode();
        }

        if (path == this->data()->m_DocumentElement.name())
        {
            return this->data()->m_DocumentElement;
        }

        return this->data()->m_DocumentElement.select_single_node(path);
    }

    Collections::List<XmlNode> XmlDocument::select_nodes(const Eden::Text::String& path) const
    {
        if (this->data()->m_DocumentElement.is_null())
        {
            return Collections::List<XmlNode>();
        }

        return this->data()->m_DocumentElement.select_nodes(path);
    }

    XmlDocument XmlDocument::Load(const Eden::IO::Path& path)
    {
        XmlDocument document;
        document.load(path);
        return document;
    }

    XmlDocument XmlDocument::Parse(const Eden::Text::String& xml)
    {
        XmlDocument document;
        document.load_xml(xml);
        return document;
    }

    XmlDocument XmlDocument::Create(const Eden::Text::String& rootName)
    {
        XmlDocument document;
        document.set_document_element(XmlNode::Element(rootName));
        return document;
    }
}
