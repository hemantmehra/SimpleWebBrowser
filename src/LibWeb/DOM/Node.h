#pragma once

namespace Web::DOM {
    enum class NodeType {
        INVALID = 0,
        ELEMENT_NODE = 1,
        ATTRIBUTE_NODE = 2,
        TEXT_NODE = 3,
        CDATA_SECTION_NODE = 4,
        ENTITY_REFERENCE_NODE = 5,
        ENTITY_NODE = 6,
        PROCESSING_INSTRUCTION_NODE = 7,
        COMMENT_NODE = 8,
        DOCUMENT_NODE = 9,
        DOCUMENT_TYPE_NODE = 10,
        DOCUMENT_FRAGMENT_NODE = 11,
        NOTATION_NODE = 12
    };

    class Node
    {
    public:
        Node(NodeType type);
        
        NodeType type() const { return m_type; }
        bool is_element() const { return type() == NodeType::ELEMENT_NODE; }
        bool is_text() const { return type() == NodeType::TEXT_NODE; }
        bool is_document() const { return type() == NodeType::DOCUMENT_NODE; }
        bool is_document_type() const { return type() == NodeType::DOCUMENT_TYPE_NODE; }
        bool is_comment() const { return type() == NodeType::COMMENT_NODE; }
        bool is_character_data() const { return type() == NodeType::TEXT_NODE || type() == NodeType::COMMENT_NODE; }
        bool is_document_fragment() const { return type() == NodeType::DOCUMENT_FRAGMENT_NODE; }
        bool is_parent_node() const { return is_element() || is_document() || is_document_fragment(); }
        bool is_slottable() const { return is_element() || is_text(); }
        bool is_attribute() const { return type() == NodeType::ATTRIBUTE_NODE; }
        bool is_cdata_section() const { return type() == NodeType::CDATA_SECTION_NODE; }

    private:
        NodeType m_type { NodeType::INVALID };
    };
}