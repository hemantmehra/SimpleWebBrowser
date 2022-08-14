#include <HTMLParser.h>
#include <DOM/DocumentType.h>

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
                if (token.type() == HTMLToken::Type::DOCTYPE)
                {
                    auto doctype = new DOM::DocumentType(document());
                    doctype->set_name(token.m_doctype.name);
                    m_document->append_child(doctype);
                    m_insertion_mode = InsertionMode::BeforeHtml;
                }
                break;
            
            case InsertionMode::BeforeHtml:
                break;
            
            default:
                assert(false);
                break;
            }
        }
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