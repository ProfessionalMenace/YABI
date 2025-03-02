#include "interpreter.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <print>

int main() {
    std::string input;
    std::getline(std::cin, input);
    remove_invalid_characters(input);
    if(!check_parenthesation(input)) {
        std::cerr << "Err: Invalid parenthesis\n";
        return EXIT_FAILURE;
    }
    auto tokens = tokenize(input);
    print_tokens(tokens);
    Interpreter foo;
    foo.interpret(tokens);
}
