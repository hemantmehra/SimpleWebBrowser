#pragma once

#include <string>
#include <DOM/CharacterData.h>

namespace Web::DOM {
    class Text : public CharacterData
    {
    public:
        Text(Document& document, std::string data)
            : CharacterData(document, NodeType::TEXT_NODE, data)
        {}

        std::string tag_name() override { return ""; }
        std::string to_string() override { return "#text: " + data(); }
    };
}