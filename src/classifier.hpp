#include <iostream>
#include <print>
#include <stack>
#include <string>
#include <vector>

enum class Symbol { bad, numeric, movl, movr, inc, dec, out, in, loop_begin, loop_end };
struct Token {
    Symbol type;
    size_t data;
};

[[nodiscard]] Symbol classify(char ch);
[[nodiscard]] std::vector<Token> tokenize(const std::string &str); 
void print_tokens(const std::vector<Token> &tokens); 
