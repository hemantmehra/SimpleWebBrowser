#include <DOM/ElementFactory.h>

namespace Web::DOM {
    Element* create_element(Document& document, std::string tag_name)
    {
        return (new Element(document, tag_name));
    }
}