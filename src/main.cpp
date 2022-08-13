#include <iostream>
#include <HTMLToken.h>
#include <HTMLTokenizer.h>

int main(int argc, char const *argv[])
{
    Web::HTMLTokenizer html_tokenizer("<!DOCTYPE html>");
    html_tokenizer.source();
    std::cout << std::string(50, '-') << std::endl;
    Web::HTMLToken token;
    do
    {
        token = html_tokenizer.next_token();
        std::cout << token.to_string() << std::endl;
    } while (token.to_string() != "EndOfFile");

    return 0;
}
