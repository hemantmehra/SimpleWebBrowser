#pragma once

#include <string>
#include <Forward.h>

namespace Web {
    void dump_tree(std::string&, DOM::Node&, size_t indent);
}