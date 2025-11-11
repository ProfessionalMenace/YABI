import classifier;
import interpreter;
#include <iostream>
#include <print>

int main() {
    std::string input;
    std::getline(std::cin, input, '\0');
    auto tokens = tokenize(input);
    Interpreter foo(100000);
    foo.interpret(tokens);
}
