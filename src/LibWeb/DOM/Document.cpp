#include <DOM/Node.h>
#include <DOM/Document.h>

namespace Web::DOM {
    Document::Document() : Node(*this, NodeType::DOCUMENT_NODE)
    {}

    void Document::append_child(Node* node)
    {
        m_children.push_back(node);
    }
}