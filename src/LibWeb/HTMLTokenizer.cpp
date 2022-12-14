#include <HTMLTokenizer.h>

// #define TOKENIZER_TRACE

#define ANYTHING_ELSE 1

#define CURRENT_INPUT_CHAR_IS_EOF (!current_input_char.has_value())
#define CURRENT_INPUT_CHAR_IS(ch) (current_input_char.has_value() && current_input_char.value() == ch)
#define CURRENT_INPUT_CHAR_IS_ALPHA (current_input_char.has_value() && isalpha(current_input_char.value()))
#define CURRENT_INPUT_CHAR_IS_UPPER_ALPHA (current_input_char.has_value() && isupper(current_input_char.value()))
#define CURRENT_INPUT_CHAR_IS_LOWER_ALPHA (current_input_char.has_value() && islower(current_input_char.value()))
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
    will_emit(m_current_token);    \
    return m_current_token;

#define EMIT_TOKEN \
    will_emit(m_current_token);    \
    return m_current_token;

#define EMIT_CHARACTER(character) \
    SET_CURRENT_TOKEN(HTMLToken::Type::Character); \
    m_current_token.m_comment_or_character.data.push_back(character); \
    EMIT_TOKEN;

#define EMIT_CURRENT_CHARACTER \
    EMIT_CHARACTER(current_input_char.value());

#define SET_STATE_AND_EMIT_TOKEN(state) \
    m_state = state; \
    will_emit(m_current_token);    \
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
                    EMIT_CURRENT_CHARACTER;
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

                if (match_next_chars("--")) {
                    consume_chars("--");
                    SET_CURRENT_TOKEN(HTMLToken::Type::Comment);
                    m_current_token.m_comment_or_character.data = "";
                    SET_STATE_AND_BREAK(State::CommentStart);
                }

                assert(false);
                break;
            
            case State::CommentStart:
                if (ANYTHING_ELSE) {
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::Comment);
                }
                assert(false);
                break;
            
            case State::Comment:
                if (CURRENT_INPUT_CHAR_IS('-')) {
                    SET_STATE_AND_BREAK(State::CommentEndDash);
                }

                if (ANYTHING_ELSE) {
                    m_current_token.m_comment_or_character.data.push_back(current_input_char.value());
                    continue;
                }
                assert(false);
                break;
            
            case State::CommentEndDash:
                if (CURRENT_INPUT_CHAR_IS('-')) {
                    SET_STATE_AND_BREAK(State::CommentEnd);
                }
                assert(false);
                break;

            case State::CommentEnd:
                if (CURRENT_INPUT_CHAR_IS('>')) {
                    SET_STATE_AND_EMIT_TOKEN(State::Data);
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

            case State::RCDATA:
                if (CURRENT_INPUT_CHAR_IS('&')) {
                    m_return_state = State::RCDATA;
                    SET_STATE_AND_BREAK(State::CharacterReference);
                }

                if (CURRENT_INPUT_CHAR_IS('<')) {
                    SET_STATE_AND_BREAK(State::RCDATALessThanSign);
                }

                if (CURRENT_INPUT_CHAR_IS(0)) {
                    assert(false);
                }

                if (CURRENT_INPUT_CHAR_IS_EOF) {
                    SET_AND_EMIT_TOKEN(HTMLToken::Type::EndOfFile);
                }

                if (ANYTHING_ELSE) {
                    EMIT_CURRENT_CHARACTER;
                }
                break;
            
            case State::RCDATALessThanSign:
                if (CURRENT_INPUT_CHAR_IS('/')) {
                    m_temporary_buffer.clear();
                    SET_STATE_AND_BREAK(State::RCDATAEndTagOpen);
                }

                if (ANYTHING_ELSE) {
                    EMIT_CHARACTER('<');
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::RCDATA);
                }
                assert(false);
                break;
            
            case State::RCDATAEndTagOpen:
                if (CURRENT_INPUT_CHAR_IS_ALPHA) {
                    SET_CURRENT_TOKEN(HTMLToken::Type::EndTag);
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::RCDATAEndTagName);
                }

                if (ANYTHING_ELSE) {
                    assert(false);
                }
                assert(false);
                break;
            
            case State::RCDATAEndTagName:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) {
                    assert(false);
                }

                if (CURRENT_INPUT_CHAR_IS('/')) assert(false);

                if (CURRENT_INPUT_CHAR_IS('>')) {
                    if (!current_end_tag_token_is_appropriate()) {
                        assert(false);
                    }
                    SET_STATE_AND_EMIT_TOKEN(State::Data);
                }

                if (CURRENT_INPUT_CHAR_IS_UPPER_ALPHA) {
                    m_current_token.m_tag.tag_name.push_back(tolower(current_input_char.value()));
                    m_temporary_buffer.push_back(current_input_char.value());
                    continue;
                }

                if (CURRENT_INPUT_CHAR_IS_LOWER_ALPHA) {
                    m_current_token.m_tag.tag_name.push_back(current_input_char.value());
                    m_temporary_buffer.push_back(current_input_char.value());
                    continue;
                }

                if (ANYTHING_ELSE) assert(false);

                assert(false);
                break;
            
            case State::RAWTEXT:
                if (CURRENT_INPUT_CHAR_IS('<')) {
                    SET_STATE_AND_BREAK(State::RAWTEXTLessThanSign);
                }

                if (CURRENT_INPUT_CHAR_IS(0)) {
                    assert(false);
                }

                if (CURRENT_INPUT_CHAR_IS_EOF) {
                    SET_AND_EMIT_TOKEN(HTMLToken::Type::EndOfFile);
                }

                if (ANYTHING_ELSE) {
                    EMIT_CURRENT_CHARACTER;
                }
                break;
            
            case State::RAWTEXTLessThanSign:
                if (CURRENT_INPUT_CHAR_IS('/')) {
                    m_temporary_buffer.clear();
                    SET_STATE_AND_BREAK(State::RAWTEXTEndTagOpen);
                }

                if (ANYTHING_ELSE) {
                    EMIT_CHARACTER('<');
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::RAWTEXT);
                }
                assert(false);
                break;
            
            case State::RAWTEXTEndTagOpen:
                if (CURRENT_INPUT_CHAR_IS_ALPHA) {
                    SET_CURRENT_TOKEN(HTMLToken::Type::EndTag);
                    SET_STATE_DEC_CURSOR_AND_BREAK(State::RAWTEXTEndTagName);
                }

                if (ANYTHING_ELSE) {
                    assert(false);
                }
                assert(false);
                break;
            
            case State::RAWTEXTEndTagName:
                if (CURRENT_INPUT_CHAR_IS_WHITESPACE) {
                    assert(false);
                }

                if (CURRENT_INPUT_CHAR_IS('/')) assert(false);

                if (CURRENT_INPUT_CHAR_IS('>')) {
                    if (!current_end_tag_token_is_appropriate()) {
                        assert(false);
                    }
                    SET_STATE_AND_EMIT_TOKEN(State::Data);
                }

                if (CURRENT_INPUT_CHAR_IS_UPPER_ALPHA) {
                    m_current_token.m_tag.tag_name.push_back(tolower(current_input_char.value()));
                    m_temporary_buffer.push_back(current_input_char.value());
                    continue;
                }

                if (CURRENT_INPUT_CHAR_IS_LOWER_ALPHA) {
                    m_current_token.m_tag.tag_name.push_back(current_input_char.value());
                    m_temporary_buffer.push_back(current_input_char.value());
                    continue;
                }

                if (ANYTHING_ELSE) assert(false);

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

    bool HTMLTokenizer::current_end_tag_token_is_appropriate()
    {
        assert(m_current_token.is_end_tag());
        if (!m_last_emitted_start_tag.is_start_tag()) {
            return false;
        }
        return (m_last_emitted_start_tag.tag_name() == m_current_token.tag_name());
    }

    void HTMLTokenizer::will_emit(HTMLToken& token)
    {
        if (token.is_start_tag()) {
            m_last_emitted_start_tag = token;
        }
    }

    void HTMLTokenizer::switch_to(State new_state)
    {
#ifdef TOKENIZER_TRACE
        std::cout << ">> "<< state_name(m_state) << " Parser switching tokenizer state from parser to " << state_name(new_state) << std::endl;
#endif
        m_state = new_state;
    }
}