#include <Dump.h>
#include <DOM/Node.h>
#include <DOM/Document.h>

namespace Web {
    void dump_document(std::string& str, DOM::Document& document) {
        str.append("Document\n");
            for(auto i : document.children()) {
                str.append(" doctype\n");
            }
    }
}