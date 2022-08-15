#include <iostream>
#include <sstream>
#include <fstream>
#include <Dump.h>
#include <HTMLParser.h>

int main(int argc, char const *argv[])
{

    if (argc < 2) {
        std::cout << "Simple Web Browser" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Usage swb <filename>" << std::endl;
        return 0;
    }

    // Read html file
    std::string filename = argv[1];
    std::ifstream file_stream(filename);
    std::stringstream buffer;
    while(file_stream >> buffer.rdbuf());

    // Print source
    std::string source = buffer.str();
    std::cout << source << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    // Parse source
    Web::HTMLParser html_parser(source);
    html_parser.run();
    std::cout << std::string(50, '-') << std::endl;

    // Print parse tree
    std::string s;
    Web::dump_tree(s, html_parser.document(), 0);
    std::cout << s << std::endl;

    return 0;
}
