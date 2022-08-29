#pragma once

#include <string>
#include <DOM/CharacterData.h>

namespace Web::DOM {
    class Comment : public CharacterData
    {
    public:
        Comment(Document& document, std::string data)
            : CharacterData(document, NodeType::COMMENT_NODE, data)
        {}

        std::string tag_name() override { return ""; }
        std::string to_string() override { return "#comment: " + data(); }
    };
}