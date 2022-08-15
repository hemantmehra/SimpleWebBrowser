#include <DOM/Element.h>

namespace Web::DOM {
    
    Element::Element(Document& document, std::string tag_name)
        : Node(document, NodeType::ELEMENT_NODE)
        , m_tag_name(tag_name)
    {}

    std::string Element::to_string()
    {
        std::string s;
        s.append("<" + m_tag_name);
        for(auto& attribute : m_attributes) {
            s.append(" " + attribute.first + "=" + "\"" + attribute.second + "\"");
        }
        s.append(">");
        return s;
    }
    void Element::set_attribute(std::pair<std::string, std::string> attribute)
    {
        m_attributes.push_back(attribute);
    }
}