#include <string>
#include <vector>
#include <HTMLTokenizer.h>
#include <DOM/Node.h>
#include <DOM/Element.h>
#include <DOM/Document.h>

#define ENUMERATE_INSERTION_MODES \
        __ENUMERATE_INSERTION_MODE(Initial) \
        __ENUMERATE_INSERTION_MODE(BeforeHtml) \
        __ENUMERATE_INSERTION_MODE(BeforeHead) \
        __ENUMERATE_INSERTION_MODE(InHead) \
        __ENUMERATE_INSERTION_MODE(InHeadNoscript) \
        __ENUMERATE_INSERTION_MODE(AfterHead) \
        __ENUMERATE_INSERTION_MODE(InBody) \
        __ENUMERATE_INSERTION_MODE(Text) \
        __ENUMERATE_INSERTION_MODE(InTable) \
        __ENUMERATE_INSERTION_MODE(InTableText) \
        __ENUMERATE_INSERTION_MODE(InCaption) \
        __ENUMERATE_INSERTION_MODE(InColumnGroup) \
        __ENUMERATE_INSERTION_MODE(InTableBody) \
        __ENUMERATE_INSERTION_MODE(InRow) \
        __ENUMERATE_INSERTION_MODE(InCell) \
        __ENUMERATE_INSERTION_MODE(InSelect) \
        __ENUMERATE_INSERTION_MODE(InSelectInTable) \
        __ENUMERATE_INSERTION_MODE(InTemplate) \
        __ENUMERATE_INSERTION_MODE(AfterBody) \
        __ENUMERATE_INSERTION_MODE(InFrameset) \
        __ENUMERATE_INSERTION_MODE(AfterFrameset) \
        __ENUMERATE_INSERTION_MODE(AfterAfterBody) \
        __ENUMERATE_INSERTION_MODE(AfterAfterFrameset) \

namespace Web
{
    class HTMLParser
    {
    public:
        explicit HTMLParser(const std::string& input);
        ~HTMLParser();

        void run();

        enum class InsertionMode
        {
#define __ENUMERATE_INSERTION_MODE(mode) mode,
            ENUMERATE_INSERTION_MODES
#undef __ENUMERATE_INSERTION_MODE
        };

        InsertionMode insertion_mode() const { return m_insertion_mode; }
        DOM::Document& document();

    private:
        void handle_initial(HTMLToken&);
        void handle_before_html(HTMLToken&);
        void handle_before_head(HTMLToken&);
        void handle_in_head(HTMLToken&);
        void handle_after_head(HTMLToken&);
        void handle_in_body(HTMLToken&);
        void handle_after_body(HTMLToken&);
        void handle_after_after_body(HTMLToken&);

        DOM::Node* current_node();
        DOM::Node* find_appropriate_place_for_inserting_node();
        DOM::Element* create_element_for(HTMLToken& token);
        DOM::Element* insert_html_element(HTMLToken& token);
        void generate_implied_end_tags();
        bool stack_of_open_elements_has_element_with_tag_name_in_scope(std::string tag_name);

        const char* insertion_mode_name() const;
        InsertionMode m_insertion_mode{ InsertionMode::Initial };
        std::vector<DOM::Node*> m_stack_of_open_elements;

        HTMLTokenizer m_tokenizer;
        DOM::Document* m_document;
        DOM::Element* m_head_element;
        DOM::Element* m_form_element;

        bool m_foster_parenting{ false };
        bool m_frameset_ok{ true };
        bool m_parsing_fragment{ false };
    };
}