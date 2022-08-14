#include <DOM/Node.h>
#include <DOM/DocumentType.h>

namespace Web::DOM {
    DocumentType::DocumentType() : Node(NodeType::DOCUMENT_TYPE_NODE)
    {}
}