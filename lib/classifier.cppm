module;
#include <cstdint>
#include <iostream>
#include <print>
#include <stack>
#include <string>
#include <vector>
export module classifier;

export enum class Symbol { bad, movl, movr, inc, dec, out, in, loop_begin, loop_end };

export struct Token {
    Symbol type;
    uint8_t data;
};

export [[nodiscard]] Symbol classify(char ch) {
    using enum Symbol;
    switch (ch) {
    case '<': return movl;
    case '>': return movr;
    case '+': return inc;
    case '-': return dec;
    case '.': return out;
    case ',': return in;
    case '[': return loop_begin;
    case ']': return loop_end;
    default: return bad;
    }
}

export [[nodiscard]] std::vector<Token> tokenize(const std::string &str) {
    std::vector<Token> tokens;
    std::stack<size_t> labels;
    bool stop_flag = false;

    // in case of error stop tokenizer
    auto stop_tokenizer = [&stop_flag](auto message) {
        std::println(std::cerr, "Stopping tokenizer! {}", message);
        stop_flag = true;
    };

    for (size_t idx = 0; !stop_flag && idx < str.size(); ++idx) {
        auto symbol = classify(str[idx]);

        switch (symbol) {
        case Symbol::bad: /* ignore bad symbols */ break;
        case Symbol::loop_end:
            if (labels.empty()) {
                stop_tokenizer("Unclosed right bracket");
            } else {
                size_t value = labels.top();
                tokens[value].data = tokens.size(); // loop begin
                tokens.emplace_back(symbol, value); // loop end
                labels.pop();
            }
            break;
        case Symbol::loop_begin:
            labels.push(tokens.size());
            tokens.emplace_back(symbol, 0);
            break;
        default:
            if (!tokens.empty() && symbol == tokens.back().type) {
                tokens.back().data += 1;
            } else {
                tokens.emplace_back(symbol, 1);
            }
        }
    }

    if (!labels.empty()) {
        stop_tokenizer("Unclosed left bracket");
    }

    if (!stop_flag) {
        return tokens;
    } else {
        return {};
    }
}

void print_tokens(const std::vector<Token> &tokens) {
    char chars[] = "  <>+-.,[]";
    for (size_t i = 0; i < tokens.size(); ++i) {
        auto tok = tokens[i];
        std::println("Token[{}]: ({}, {})", i, chars[static_cast<size_t>(tok.type)], tok.data);
    }
}
