#include <HTMLParser.h>
#include <DOM/DocumentType.h>
#include <DOM/ElementFactory.h>

namespace Web
{
    HTMLParser::HTMLParser(const std::string& input)
        : m_tokenizer(input)
    {}

    HTMLParser::~HTMLParser()
    {}

    void HTMLParser::run()
    {
        m_document = new DOM::Document;
        for (;;)
        {
            auto token = m_tokenizer.next_token();
            if (token.to_string() == "EndOfFile") return;

            std::cout << "[" << insertion_mode_name() << "] " << token.to_string() << std::endl;

            if (token.type() == HTMLToken::Type::EndOfFile)
                return;

            switch (m_insertion_mode)
            {
            case InsertionMode::Initial:
                handle_initial(token);
                break;
            
            case InsertionMode::BeforeHtml:
                handle_before_html(token);
                break;

            case InsertionMode::BeforeHead:
                handle_before_head(token);
                break;
            
            case InsertionMode::InHead:
                // handle_in_head(token);
                break;
            
            default:
                assert(false);
            }
        }
    }

    void HTMLParser::handle_initial(HTMLToken& token)
    {
        if (token.type() == HTMLToken::Type::DOCTYPE)
        {
            auto doctype = new DOM::DocumentType(document());
            doctype->set_name(token.m_doctype.name);
            document().append_child(doctype);
            m_insertion_mode = InsertionMode::BeforeHtml;
            return;
        }
        assert(false);
    }

    void HTMLParser::handle_before_html(HTMLToken& token)
    {
        if (token.is_start_tag() && token.tag_name() == "html") {
            auto element = DOM::create_element(document(), token.tag_name());
            document().append_child(element);
            m_stack_of_open_elements.push_back(element);
            m_insertion_mode = InsertionMode::BeforeHead;
            return;
        }
        assert(false);
    }

    DOM::Node* HTMLParser::find_appropriate_place_for_inserting_node()
    {
        auto target = current_node();
        if (m_foster_parenting)
            assert(false);
        return target;
    }

    DOM::Node* HTMLParser::current_node()
    {
        return m_stack_of_open_elements.back();
    }

    DOM::Element* HTMLParser::insert_html_element(HTMLToken& token)
    {
        auto adjusted_insertion_location = find_appropriate_place_for_inserting_node();
        auto element = DOM::create_element(document(), token.tag_name());
        adjusted_insertion_location->append_child(element);
        m_stack_of_open_elements.push_back(element);
        return element;
    }

    void HTMLParser::handle_before_head(HTMLToken& token)
    {
        if (token.is_start_tag() && token.tag_name() == "head") {
            auto element = insert_html_element(token);
            m_head_element = element;
            m_insertion_mode = InsertionMode::InHead;
            return;
        }
        assert(false);
    }

    const char* HTMLParser::insertion_mode_name() const
    {
        switch (m_insertion_mode)
        {
#define __ENUMERATE_INSERTION_MODE(mode) case InsertionMode::mode: return #mode;
        ENUMERATE_INSERTION_MODES
#undef __ENUMERATE_INSERTION_MODE
        }
        assert(false);
    }

    DOM::Document& HTMLParser::document()
    {
        return *m_document;
    }
}