#pragma once

#include <DOM/Node.h>

namespace Web::DOM {
    class Element : public Node
    {
    public:
        Element(Document&, std::string tag_name);
        std::string to_string() override { return "<" + m_tag_name + ">"; }
    
    protected:
        std::string m_tag_name;
    private:
    };
}