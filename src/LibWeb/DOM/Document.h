#pragma once

#include <vector>
#include <DOM/Node.h>

namespace Web::DOM {
    class Document : public Node
    {
    public:
        Document();
        void append_child(Node*);
        std::vector<Node*>& children() { return m_children; }

    private:
        std::vector<Node*> m_children;
    };
}