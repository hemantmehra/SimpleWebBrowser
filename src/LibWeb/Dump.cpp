#include <Dump.h>
#include <DOM/Node.h>
#include <DOM/Document.h>
#include <DOM/DocumentType.h>

namespace Web {
    void dump_tree(std::string& str, DOM::Node& node, size_t indent) {
        if (node.is_document()) {
            str.append(std::string(2 * indent, ' '));
            str.append(node.to_string());
            str.append("\n");

            for(auto i : node.child_nodes()) {
                dump_tree(str, *i, indent + 1);
            }
        }

        if (node.is_document_type()) {
            str.append(std::string(2 * indent, ' '));
            str.append(node.to_string());
            str.append("\n");
        }

        if (node.is_element()) {
            str.append(std::string(2 * indent, ' '));
            str.append(node.to_string());
            str.append("\n");

            for(auto i : node.child_nodes()) {
                dump_tree(str, *i, indent + 1);
            }
        }

        if (node.is_text()) {
            str.append(std::string(2 * indent, ' '));
            str.append(node.to_string());
            str.append("\n");

            for(auto i : node.child_nodes()) {
                dump_tree(str, *i, indent + 1);
            }
        }

    }
}