#include <DOM/Node.h>
#include <DOM/DocumentType.h>

namespace Web::DOM {
    DocumentType::DocumentType(Document& document)
        : Node(document, NodeType::DOCUMENT_TYPE_NODE)
    {}
}