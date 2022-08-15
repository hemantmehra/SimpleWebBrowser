add_library(LibWeb "")

target_sources(LibWeb
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/HTMLTokenizer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HTMLToken.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HTMLParser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Dump.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DOM/Element.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DOM/ElementFactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DOM/Node.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DOM/Document.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DOM/DocumentType.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/HTMLTokenizer.h
    ${CMAKE_CURRENT_LIST_DIR}/HTMLToken.h
    ${CMAKE_CURRENT_LIST_DIR}/HTMLParser.h
    ${CMAKE_CURRENT_LIST_DIR}/Dump.h
    ${CMAKE_CURRENT_LIST_DIR}/DOM/Element.h
    ${CMAKE_CURRENT_LIST_DIR}/DOM/ElementFactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DOM/Node.h
    ${CMAKE_CURRENT_LIST_DIR}/DOM/Document.h
    ${CMAKE_CURRENT_LIST_DIR}/DOM/DocumentType.h
  )

target_include_directories(LibWeb
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )