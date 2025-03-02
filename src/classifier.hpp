#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <print>

enum struct Symbol {
    bad, // bad symbol
    numeric, // digit or a number
    movl,
    movr,
    inc,
    dec,
    out,
    in,
    loop_begin,
    loop_end
};

[[nodiscard]]
Symbol classify(char ch) noexcept {
    using enum Symbol;
    if(ch >= '0' && ch <= '9') {
        return numeric;
    }
    switch(ch) {
        case '<': return movl;
        case '>': return movr;
        case '+': return inc;
        case '-': return dec;
        case '.': return out;
        case ',': return in;
        case '[': return loop_begin;
        case ']': return loop_end;
        default : return bad;
    }
}

struct Token {
    Symbol type;
    size_t data; // pos or value
};

[[nodiscard]]
std::vector<Token> tokenize(const std::string &str) {
    std::vector<Token> tokens;
    std::stack<size_t> labels;
    size_t value = 0;

    auto handle_unused = [&value](auto message){ 
        if (value) {
            std::cerr << "ignored value: " << message << '\n';
            value = 0;
        }
    };

    for(auto ch : str) {
        auto symbol = classify(ch);
        switch (symbol) {
            case Symbol::bad:
                std::cerr << "Bad symbol ignored\n";
                break;
            case Symbol::numeric:
                value = value*10 + (ch - '0');
                break;
            case Symbol::loop_end:
                handle_unused("loop_end");
                if(!labels.size()) {
                    std::cerr << "Unclosed right bracket\n";
                    return tokens;
                }
                value = labels.top();
                tokens[value].data = tokens.size();
                tokens.emplace_back(symbol, value);
                labels.pop();
                break;
            case Symbol::loop_begin:
                handle_unused("loop_begin");
                labels.push(tokens.size());
                [[fallthrough]];
            default: 
                if(value == 0) { value = 1; }
                tokens.emplace_back(symbol, value);
                value = 0;
                break;
        }
    }
    if (labels.size()) {
        std::cerr << "Unclosed left bracket\n";
    }
    return tokens;
}

void print_tokens(const std::vector<Token> &tokens) {
    char chars[] = "  <>+-,.[]";
    for(size_t i = 0; i < tokens.size(); ++i) {
        auto tok = tokens[i];
        std::println("Token[{}]: ({}, {})", i, chars[static_cast<size_t>(tok.type)], tok.data);
    }
}

[[maybe_unused]]
auto remove_invalid_characters(std::string &str) noexcept {
    std::erase_if(str, [](char &ch){ return classify(ch) == Symbol::bad; });
    return str.size();
}

[[maybe_unused, nodiscard]]
bool check_parenthesation(const std::string &str) noexcept {
    size_t left = 0;
    for(char ch : str) {
        auto value = classify(ch);
        if(value == Symbol::loop_begin) {
            ++left;
        } else if(value == Symbol::loop_end) {
            if(left == 0) { return false; }
            --left;
        }
    }
    return (left == 0);
}
