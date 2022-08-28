#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <Forward.h>

namespace Web {
    class HTMLToken
    {
        friend class HTMLTokenizer;
        friend class HTMLParser;
    public:
        enum class Type
        {
            Invalid,
            DOCTYPE,
            StartTag,
            EndTag,
            Comment,
            Character,
            EndOfFile
        };

        bool is_doctype () const { return type() == Type::DOCTYPE; }
        bool is_start_tag() const { return type() == Type::StartTag; }
        bool is_end_tag() const { return type() == Type::EndTag; }
        bool is_comment() const { return type() == Type::Comment; }
        bool is_character() const { return type() == Type::Character; }
        bool is_end_of_file () const { return type() == Type::EndOfFile; }

        uint32_t character() const
        {
            assert(is_character());
            assert(m_comment_or_character.data.length() == 1);
            return m_comment_or_character.data[0];
        }

        bool is_parser_whitespace() const
        {
            if (!is_character()) {
                return false;
            }
            
            switch (character())
            {
            case '\t':
            case '\n':
            case '\f':
            case '\r':
            case ' ':
                return true;
            default:
                return false;
            }
        }

        std::string tag_name() const
        {
            assert(is_start_tag() || is_end_tag());
            return m_tag.tag_name;
        }

        bool is_self_closing() const
        {
            assert(is_start_tag() || is_end_tag());
            return m_tag.self_closing;
        }

        Type type() const { return m_type; }
        std::string to_string() const;

    private:
        Type m_type{ Type::Invalid };

        struct {
            std::string name;
            std::string public_identifier;
            std::string system_identifier;
            bool force_quirks{ false };
        } m_doctype;

        struct {
            std::string tag_name;
            bool self_closing{ false };
            std::vector<std::pair<std::string, std::string>> attributes;
        } m_tag;

        struct {
            std::string data;
        } m_comment_or_character;
    };
}