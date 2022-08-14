#include <Dump.h>
#include <DOM/Node.h>
#include <DOM/Document.h>

namespace Web {
    void dump_tree(std::string& str, DOM::Node& node) {
        if (node.is_document()) {
            str.append("Document\n");
        }
    }
}