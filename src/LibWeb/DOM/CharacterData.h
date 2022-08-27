#pragma once

#include <string>
#include <DOM/Node.h>

namespace Web::DOM {
    class CharacterData : public Node
    {
    public:
        CharacterData(Document& document, NodeType type, std::string data)
            : Node(document, type)
            , m_data(data)
        {}

        const std::string& data() const { return m_data; }
        void set_data(const std::string& data) { m_data = data; }

    private:
        std::string m_data;
    };
}