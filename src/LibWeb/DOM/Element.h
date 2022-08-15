#pragma once

#include <vector>
#include <DOM/Node.h>

namespace Web::DOM {
    class Element : public Node
    {
    public:
        Element(Document&, std::string tag_name);
        std::string tag_name() override { return m_tag_name; }
        std::string to_string() override;
        void set_attribute(std::pair<std::string, std::string> attribute);
    
    protected:
        std::string m_tag_name;
        std::vector<std::pair<std::string, std::string>> m_attributes;
    private:
    };
}