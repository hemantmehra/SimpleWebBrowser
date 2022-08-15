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
            std::cout << "[" << insertion_mode_name() << "] " << token.to_string() << std::endl;

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
                handle_in_head(token);
                break;
            
            case InsertionMode::AfterHead:
                handle_after_head(token);
                break;

            case InsertionMode::InBody:
                handle_in_body(token);
                break;
            
            case InsertionMode::AfterBody:
                handle_after_body(token);
                break;

            case InsertionMode::AfterAfterBody:
                handle_after_after_body(token);
                break;
            
            default:
                assert(false);
            }

            if (token.type() == HTMLToken::Type::EndOfFile)
                return;
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
            auto element = create_element_for(token);
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

    DOM::Element* HTMLParser::create_element_for(HTMLToken& token)
    {
        auto element = DOM::create_element(document(), token.tag_name());
        for (auto& attribute : token.m_tag.attributes) {
            element->set_attribute(attribute);
        }
        return element;
    }

    DOM::Element* HTMLParser::insert_html_element(HTMLToken& token)
    {
        auto adjusted_insertion_location = find_appropriate_place_for_inserting_node();
        auto element = create_element_for(token);
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

    void HTMLParser::handle_in_head(HTMLToken& token)
    {
        if (token.is_start_tag() && token.tag_name() == "meta") {
            auto element = insert_html_element(token);
            m_stack_of_open_elements.pop_back();
            if (token.is_self_closing()) {
                assert(false);
            }
            return;
        }

        if (token.is_end_tag() && token.tag_name() == "head") {
            m_stack_of_open_elements.pop_back();
            m_insertion_mode = InsertionMode::AfterHead;
            return;
        }
        assert(false);
    }

    void HTMLParser::handle_after_head(HTMLToken& token)
    {
        if (token.is_character()) {
            assert(false);
        }

        if (token.is_comment()) {
            assert(false);
        }

        if (token.is_doctype()) {
            assert(false);
        }

        if (token.is_start_tag() && token.tag_name() == "html") {
            assert(false);
        }

        if (token.is_start_tag() && token.tag_name() == "body") {
            auto element = insert_html_element(token);
            m_frameset_ok = false;
            m_insertion_mode = InsertionMode::InBody;
            return;
        }

        if (token.is_start_tag() && token.tag_name() == "frameset") {
            assert(false);
        }

        {
            std::vector names = {"base", "basefont", "bgsound", "link", "meta", "noframes", "script", "style", "template", "title"};
            if (std::find(names.begin(), names.end(), token.tag_name()) != names.end()) {
                assert(false);
            }
        }

        if (token.is_end_tag() && token.tag_name() == "template") {
            assert(false);
        }

        if (token.is_end_tag()
            && ((token.tag_name() == "body")
                || (token.tag_name() == "html")
                || (token.tag_name() == "br"))) {

            goto AythingElse;
        }

        if ((token.is_start_tag() && token.tag_name() == "head") || token.is_end_tag()) {
            assert(false);
        }

        AythingElse:
            HTMLToken dummy_body_token;
            dummy_body_token.m_type = HTMLToken::Type::StartTag;
            dummy_body_token.m_tag.tag_name.append("body");
            insert_html_element(dummy_body_token);
            m_insertion_mode = InsertionMode::InBody;
            return;
        assert(false);
    }

    void HTMLParser::generate_implied_end_tags()
    {
        std::vector names{ "dd", "dt", "li", "optgroup", "option", \
        "p", "rb", "rp", "rt", "rtc" };

        while ((std::find(names.begin(), names.end(), current_node()->tag_name()) != names.end())) {
            m_stack_of_open_elements.pop_back();
        }
    }

    bool HTMLParser::stack_of_open_elements_has_element_with_tag_name_in_scope(std::string tag_name) {
        std::vector list{ "applet", "caption", "html", "table", "td", "th", "marquee", "object", "template" };
        for (size_t i = m_stack_of_open_elements.size() - 1; i > 0; --i) {
            auto& node = m_stack_of_open_elements.at(i);
            if (node->tag_name() == tag_name)
                return true;
            if ((std::find(list.begin(), list.end(), node->tag_name()) != list.end())) {
                return false;
            }
        }
        assert(false);
    }

    void HTMLParser::handle_in_body(HTMLToken& token)
    {
        if (token.is_end_tag() && token.tag_name() == "body") {
            if (!stack_of_open_elements_has_element_with_tag_name_in_scope("body")) {
                assert(false);
            }

            m_insertion_mode = InsertionMode::AfterBody;
            return;
        }

        {
            std::vector names = {"address", "article", "aside", "blockquote", \
            "center", "details", "dialog", "dir", "div", "dl", "fieldset", "figcaption", \
            "figure", "footer", "header", "hgroup", "main", "menu", "nav", "ol", "p", "section", "summary", "ul"};
            if (token.is_start_tag() && (std::find(names.begin(), names.end(), token.tag_name()) != names.end())) {
                insert_html_element(token);
                return;
            }
        }
        
        {
            std::vector names = {"address", "article", "aside", "blockquote", \
            "button", "center", "details", "dialog", "dir", "div", "dl", "fieldset", \
            "figcaption", "figure", "footer", "header", "hgroup", "listing", "main", "menu", \
            "nav", "ol", "pre", "section", "summary", "ul"};
            if (token.is_end_tag() && (std::find(names.begin(), names.end(), token.tag_name()) != names.end())) {
                if (!stack_of_open_elements_has_element_with_tag_name_in_scope(token.tag_name())) {
                    assert(false);
                }

                generate_implied_end_tags();

                if (current_node()->tag_name() != token.tag_name()) {
                    assert(false);
                }

                m_stack_of_open_elements.pop_back();
                return;
            }
        }
        assert(false);
    }

    void HTMLParser::handle_after_body(HTMLToken& token)
    {
        if (token.is_end_tag() && token.tag_name() == "html") {
            if (m_parsing_fragment) {
                assert(false);
            }
            m_insertion_mode = InsertionMode::AfterAfterBody;
            return;
        }
        assert(false);
    }

    void HTMLParser::handle_after_after_body(HTMLToken& token)
    {
        if (token.is_end_of_file()) {
            std::cout << "End of parsing!" << std::endl;
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