#pragma once

#include <vector>
#include <DOM/Node.h>

namespace Web::DOM {
    class Document : public Node
    {
    public:
        Document();
        std::string to_string() override { return "*Document*"; }
    private:
    };
}