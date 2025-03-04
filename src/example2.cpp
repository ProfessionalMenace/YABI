#include "interpreter.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <print>

int main() {
    std::string input;
    std::getline(std::cin, input);
    auto tokens = tokenize(input);
    print_tokens(tokens);
    Interpreter foo;
    foo.interpret(tokens);
}
