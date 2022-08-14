#pragma once

#include <string>
#include <DOM/Node.h>

namespace Web::DOM {
    class DocumentType : public Node
    {
    public:
        DocumentType(Document&);
        const std::string& name() const { return m_name; }
        void set_name(const std::string& name) { m_name = name; }

    private:
        std::string m_name;
    };
}