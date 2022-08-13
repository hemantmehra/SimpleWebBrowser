add_library(LibWeb "")

target_sources(LibWeb
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/HTMLTokenizer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HTMLToken.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/HTMLTokenizer.h
    ${CMAKE_CURRENT_LIST_DIR}/HTMLToken.h
  )

target_include_directories(LibWeb
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )