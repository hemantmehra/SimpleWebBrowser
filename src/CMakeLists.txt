add_executable(swb main.cpp)

add_subdirectory(LibWeb)

target_link_libraries(swb
    PUBLIC
    LibWeb
)