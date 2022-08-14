#include <DOM/Node.h>
#include <DOM/Document.h>

namespace Web::DOM {
    Document::Document() : Node(NodeType::DOCUMENT_NODE)
    {}
}