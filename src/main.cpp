#include <iostream>
#include <HTMLParser.h>
#include <Dump.h>

int main(int argc, char const *argv[])
{
    std::string source = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"></head></html>";
    std::cout << source << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    Web::HTMLParser html_parser(source);
    html_parser.run();
    std::cout << std::string(50, '-') << std::endl;

    std::string s;
    Web::dump_tree(s, html_parser.document(), 0);
    std::cout << s << std::endl;

    return 0;
}
