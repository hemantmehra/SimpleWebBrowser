#include <DOM/Node.h>

namespace Web::DOM {
    Node::Node(Document& document, NodeType type)
        : m_document(&document)
        , m_type(type)
    {}
}