import classifier;
import interpreter;
#include <iostream>
#include <print>

int main() {
    std::string input;
    std::getline(std::cin, input);
    auto tokens = tokenize(input);
    Interpreter foo;
    foo.interpret(tokens);
}
