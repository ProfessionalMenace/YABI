#include "interpreter.hpp"
#include <cassert>
#include <cstdlib>

int main() {
    std::string input(R"(
            ++++++++
            [>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]
            >>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
            )");
    auto tokens = Tokenizer::tokenize(input);
    Tokenizer::print_tokens(tokens);
    Interpreter foo;
    foo.interpret(tokens);
}
