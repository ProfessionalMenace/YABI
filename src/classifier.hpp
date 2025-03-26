#include <iostream>
#include <print>
#include <stack>
#include <string>
#include <vector>

struct Tokenizer {
    enum Symbol { bad, numeric, movl, movr, inc, dec, out, in, loop_begin, loop_end };

    [[nodiscard]]
    static Symbol classify(char ch) {
        switch (ch) {
        // I might make this if('0' <= ch && ch <= '9') return numeric;
        case '0': return numeric;
        case '1': return numeric;
        case '2': return numeric;
        case '3': return numeric;
        case '4': return numeric;
        case '5': return numeric;
        case '6': return numeric;
        case '7': return numeric;
        case '8': return numeric;
        case '9': return numeric;
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

    struct Token {
        Symbol type;
        size_t data;
    };

    [[nodiscard]]
    static std::vector<Token> tokenize(const std::string &str) {
        std::vector<Token> tokens;
        std::stack<size_t> labels;
        size_t value = 0;
        bool stop_flag = false;

        auto stop_tokenizer = [&stop_flag](auto message) {
            std::cerr << "Stopping tokenizer... " << message << '\n';
            stop_flag = true;
        };

        auto handle_unused = [&value](auto message) {
            if (value) {
                std::cerr << "ignored value: " << message << '\n';
                value = 0;
            }
        };

        auto last_symbol = Symbol::bad;
        for (auto it = str.cbegin(); it != str.cend() && !stop_flag; ++it) {
            auto ch = *it;
            auto symbol = classify(ch);
            switch (symbol) {
            case Symbol::bad: /* ignore bad symbols */ break;
            case Symbol::numeric: value = value * 10 + (ch - '0'); break;
            case Symbol::loop_end:
                handle_unused("loop_end");
                if (!labels.size()) {
                    stop_tokenizer("Unclosed right bracket");
                    break;
                }
                value = labels.top();
                tokens[value].data = tokens.size(); // loop begin
                tokens.emplace_back(symbol, value); // loop end
                labels.pop();
                value = 0;
                break;
            case Symbol::loop_begin:
                handle_unused("loop_begin");
                labels.push(tokens.size());
                tokens.emplace_back(symbol, value);
                value = 0;
                break;
            default:
                if (symbol == last_symbol) {
                    tokens.back().data += 1;
                    break;
                }
                if (value == 0) {
                    value = 1;
                }
                tokens.emplace_back(symbol, value);
                value = 0;
                break;
            }
            last_symbol = symbol;
        }

        if (labels.size()) {
            stop_tokenizer("Unclosed left bracket");
        }

        if (!stop_flag) {
            return tokens;
        } else {
            return {};
        }
    }

    static void print_tokens(const std::vector<Token> &tokens) {
        char chars[] = "  <>+-.,[]";
        for (size_t i = 0; i < tokens.size(); ++i) {
            auto tok = tokens[i];
            std::println("Token[{}]: ({}, {})", i, chars[static_cast<size_t>(tok.type)], tok.data);
        }
    }
};
