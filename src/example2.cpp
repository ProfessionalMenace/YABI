#include "interpreter.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>

int main() {
    std::string input;
    std::getline(std::cin, input);
    auto tokens = Tokenizer::tokenize(input);
    Tokenizer::print_tokens(tokens);
    Interpreter foo;
    foo.interpret(tokens);
}
