#include <HTMLTokenizer.h>

// #define TOKENIZER_TRACE

#define ANYTHING_ELSE 1

#define CURRENT_INPUT_CHAR_IS_EOF (!current_input_char.has_value())
#define CURRENT_INPUT_CHAR_IS(ch) (current_input_char.has_value() && current_input_char.value() == ch)
#define CURRENT_INPUT_CHAR_IS_ALPHA (current_input_char.has_value() && isalpha(current_input_char.value()))
#define CURRENT_INPUT_CHAR_IS_WHITESPACE \
	(current_input_char.has_value() && \
		(current_input_char.value() == '\t') || \
		(current_input_char.value() == '\a') || \
		(current_input_char.value() == '\f') || \
		(current_input_char.value() == ' ')    \
		)

#define SET_CURRENT_TOKEN(type) \
    m_current_token = {};        \
    m_current_token.m_type = type;

#define SET_AND_EMIT_TOKEN(type)           \
    m_current_token = {};          \
    m_current_token.m_type = type; \
    return m_current_token;

#define EMIT_TOKEN \
    return m_current_token;

#define SET_STATE_AND_EMIT_TOKEN(state) \
    m_state = state; \
    return m_current_token;

// Same as switch state
#define SET_STATE_AND_BREAK(state) \
    m_state = state; \
    break;

// Same as reconsume in state
#define SET_STATE_DEC_CURSOR_AND_BREAK(state) \
    m_state = state; \
    m_cursor--;      \
    break;

#define DEC_CURSOR m_cursor--;

namespace Web {
    void HTMLTokenizer::source()
    {
        std::cout << m_input << std::endl;
    }

    std::optional<uint32_t> HTMLTokenizer::next_input_char()
    {
        if (m_cursor >= m_input.length())
            return {};
        return m_input[m_cursor++];
    }

    std::optional<uint32_t> HTMLTokenizer::peek_input_char(size_t offset) const
    {
        if ((m_cursor + offset) >= m_input.length())
            return {};
        return m_input[m_cursor + offset];
    }

    HTMLToken HTMLTokenizer::next_token()
    {
        while(1)
        {   
            auto current_input_char = next_input_char();
#ifdef TOKENIZER_TRACE
            std::cout << ">> "<< state_name(m_state) << " " << (char) (current_input_char.has_value() ? current_input_char.value() : 0) << std::endl;
#endif
            switch (m_state)
            {
            case State::Data:
                if (CURRENT_INPUT_CHAR_IS('&')) {
                    m_return_state = State::Data;
                    SET_STATE_AND_BREAK(State::CharacterReference);
                }

                if (CURRENT_INPUT_CHAR_IS('<')) {
                    SET_STATE_AND_BREAK(State::TagOpen);
                }

                if (CURRENT_INPUT_CHAR_IS_EOF) {
                    SET_AND_EMIT_TOKEN(HTMLToken::Type::EndOfFile);
                }

                if (ANYTHING_ELSE) {
                    SET_CURRENT_TOKEN(HTMLToken::Type::Character);
                    m_current_token.m_comment_or_character.data.push_back(current_input_char.value());
                    EMIT_TOKEN;
                }

                assert(false);
                break;
            
            case State::TagOpen:
                if (CURRENT_INPUT_CHAR_IS('!')) {
                    SET_STATE_AND_BREAK(State::MarkupDeclarationOpen);
                }

                if (CURRENT_INPUT_CHAR_IS('/')) {
                    SET_STATE_AND_BREAK(State::EndTagOpen);
                }

                if (CURRENT_INPUT_CHAR_IS_ALPHA) {
                    SET_CURRENT_TOKEN(HTMLToken::Type::StartTag);
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::TagName);
                }

                assert(false);
                break;

            case State::MarkupDeclarationOpen:
                DEC_CURSOR;
                if (match_next_chars("DOCTYPE")) {
                    consume_chars("DOCTYPE");
                    SET_STATE_AND_BREAK(State::DOCTYPE);
                }
                assert(false);
                break;
            
            case State::DOCTYPE:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) {
                    SET_STATE_AND_BREAK(State::BeforeDOCTYPEName);
                }

                if (CURRENT_INPUT_CHAR_IS('>')) {
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::BeforeDOCTYPEName);
                }
                assert(false);
                break;

            case State::BeforeDOCTYPEName:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) continue;

                if (ANYTHING_ELSE) {
                    SET_CURRENT_TOKEN(HTMLToken::Type::DOCTYPE);
                    m_current_token.m_doctype.name.push_back(current_input_char.value());
                    SET_STATE_AND_BREAK(State::DOCTYPEName);
                }
                assert(false);
                break;
            
            case State::DOCTYPEName:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) {
                    SET_STATE_AND_BREAK(State::AfterDOCTYPEName);
                }

                if (CURRENT_INPUT_CHAR_IS('>')) {
                    SET_STATE_AND_EMIT_TOKEN(State::Data);
                }

                if (ANYTHING_ELSE) {
                    m_current_token.m_doctype.name.push_back(current_input_char.value());
                    continue;
                }
                assert(false);
                break;
            
            case State::TagName:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) {
                    SET_STATE_AND_BREAK(State::BeforeAttributeName);
                }

                if (CURRENT_INPUT_CHAR_IS('/')) {
                    SET_STATE_AND_BREAK(State::SelfClosingStartTag);
                }

                if (CURRENT_INPUT_CHAR_IS('>')) {
                    SET_STATE_AND_EMIT_TOKEN(State::Data);
                }

                if (ANYTHING_ELSE) {
                    m_current_token.m_tag.tag_name.push_back(current_input_char.value());
                    continue;
                }
                assert(false);
                break;
            
            case State::EndTagOpen:
                if (CURRENT_INPUT_CHAR_IS_ALPHA) {
                    SET_CURRENT_TOKEN(HTMLToken::Type::EndTag);
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::TagName);
                }
                assert(false);
                break;
            
            case State::BeforeAttributeName:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) {
                    continue;
                }

                if (ANYTHING_ELSE) {
                    m_current_token.m_tag.attributes.push_back(std::make_pair("", ""));
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::AttributeName);
                }
                assert(false);
                break;
            
            case State::AttributeName:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE || CURRENT_INPUT_CHAR_IS('/') || CURRENT_INPUT_CHAR_IS('>')) {
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::AfterAttributeName);
                }

                if (CURRENT_INPUT_CHAR_IS('=')) {
                    SET_STATE_AND_BREAK(State::BeforeAttributeValue);
                }

                if (ANYTHING_ELSE) {
                    m_current_token.m_tag.attributes.back().first.push_back(current_input_char.value());
                    continue;
                }
                assert(false);
                break;
            
            case State::AfterAttributeName:
                assert(false);
                break;
            
            case State::BeforeAttributeValue:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) {
                    continue;
                }

                if (CURRENT_INPUT_CHAR_IS('\"')) {
                    SET_STATE_AND_BREAK(State::AttributeValueDoubleQuoted);
                }

                if (CURRENT_INPUT_CHAR_IS('\'')) {
                    SET_STATE_AND_BREAK(State::AttributeValueSingleQuoted);
                }

                assert(false);
                break;
            
            case State::AttributeValueDoubleQuoted:
                if (CURRENT_INPUT_CHAR_IS('\"')) {
                    SET_STATE_AND_BREAK(State::AfterAttributeValueQuoted);
                }

                if (ANYTHING_ELSE) {
                    m_current_token.m_tag.attributes.back().second.push_back(current_input_char.value());
                    continue;
                }
                assert(false);
                break;
            
            case State::AfterAttributeValueQuoted:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) {
                    SET_STATE_AND_BREAK(State::BeforeAttributeName);
                }

                if (CURRENT_INPUT_CHAR_IS('/')) {
                    SET_STATE_AND_BREAK(State::SelfClosingStartTag);
                }

                if (CURRENT_INPUT_CHAR_IS('>')) {
                    SET_STATE_AND_EMIT_TOKEN(State::Data);
                }
                assert(false);
                break;
            
            case State::SelfClosingStartTag:
                if (CURRENT_INPUT_CHAR_IS('>')) {
                    m_current_token.m_tag.self_closing = true;
                    SET_STATE_AND_EMIT_TOKEN(State::Data);
                }
                assert(false);
                break;
            
            case State::CharacterReference:
                assert(false);
                break;
            
            default:
                assert(false);
                break;
            }
        }
    }

    void HTMLTokenizer::consume_chars(const std::string& s)
	{
		assert(match_next_chars(s));
		m_cursor += s.length();
	}

    bool HTMLTokenizer::match_next_chars(const std::string& s) const
	{
		for (size_t i = 0; i < s.length(); i++) {
			auto ch = peek_input_char(i);
			if (!ch.has_value())
				return false;
			if (ch.value() != (uint32_t) s[i])
				return false;
		}
		return true;
	}
}