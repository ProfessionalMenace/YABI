#include "classifier.hpp"
#include <print>

void interpret(std::vector<Token> &tokens) {
    std::vector<char> memory_;
    size_t memory_idx = 0;
    size_t token_idx = 0;

    auto print_state= [&memory_, &memory_idx, &token_idx](){
        std::print("Memory[{}], Token[{}], Cells:", memory_idx, token_idx);
        for(auto cell : memory_) {
            std::print("[{:02x}]", cell);
        }
        std::println();
    };

    while(token_idx < tokens.size()) {
        auto token = tokens[token_idx];
        while(memory_idx >= memory_.size()) {
            memory_.push_back(0);
        }
        switch(token.type) {
            case Symbol::inc:
                memory_[memory_idx] = static_cast<char>((memory_[memory_idx] + token.data) % 256);
                ++token_idx;
                break;
            case Symbol::dec:
                memory_[memory_idx] = static_cast<char>((memory_[memory_idx] - token.data) % 256);
                ++token_idx;
                break;
            case Symbol::movr:
                memory_idx += token.data;
                ++token_idx;
                break;
            case Symbol::movl:
                if(token.data > memory_idx) { std::cerr << "Memory buffer underflow\n"; return; };
                memory_idx -= token.data;
                ++token_idx;
                break;
            case Symbol::out:
                std::cout << memory_[memory_idx];
                ++token_idx;
                break;
            case Symbol::in:
                memory_[memory_idx] = std::cin.get();
                ++token_idx;
                break;
            case Symbol::loop_begin:
                if(memory_[memory_idx] == 0) {
                    token_idx = token.data;
                }
                ++token_idx;
                break;
            case Symbol::loop_end:
                token_idx = token.data;
                break;
            case Symbol::bad: [[fallthrough]];
            case Symbol::numeric:
                std::cerr << "Unexpected symbol\n";
                ++token_idx;
        }
        print_state();
    }
};
