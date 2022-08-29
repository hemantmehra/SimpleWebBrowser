#include "HTMLToken.h"

namespace Web {
    std::string HTMLToken::to_string() const
    {
        std::string s;
        switch (type())
        {
        case HTMLToken::Type::DOCTYPE:
            s.append("DOCTYPE");
            s.append(" { name: '");
            s.append(m_doctype.name);
            s.append("' }");
            break;
        case HTMLToken::Type::StartTag:
            s.append("StartTag");
            break;
        case HTMLToken::Type::EndTag:
            s.append("EndTag");
            break;
        case HTMLToken::Type::Comment:
            s.append("Comment");
            s.append(" { ");
            s.append(m_comment_or_character.data);
            s.append(" } ");
            break;
        case HTMLToken::Type::Character:
            s.append("Character");
            break;
        case HTMLToken::Type::EndOfFile:
            s.append("EndOfFile");
            break;
        case HTMLToken::Type::Invalid:
            assert(false);
        default:
            break;
        }

        if (type() == HTMLToken::Type::StartTag || type() == HTMLToken::Type::EndTag)
        {
            s.append(" { name: '");
            s.append(m_tag.tag_name);
            s.append("'");
            for(auto i : m_tag.attributes) {
                s.append(" " + i.first + "=" + i.second + " ");
            }
            s.append(" }");
        }

        return s;
    }
}