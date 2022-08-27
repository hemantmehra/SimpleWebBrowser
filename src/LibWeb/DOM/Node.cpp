#include <DOM/Node.h>

namespace Web::DOM {
    Node::Node(Document& document, NodeType type)
        : m_document(&document)
        , m_type(type)
    {}

    void Node::append_child(Node* node)
    {
        m_last_child = node;
        m_child_nodes.push_back(node);
    }

    Node* Node::last_child()
    {
        return m_last_child;
    }
}