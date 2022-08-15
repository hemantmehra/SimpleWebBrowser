#include <DOM/Element.h>

namespace Web::DOM {
    
    Element::Element(Document& document, std::string tag_name)
        : Node(document, NodeType::ELEMENT_NODE)
        , m_tag_name(tag_name)
    {}
}