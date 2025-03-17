#include "classifier.hpp"
#include <print>

struct Interpreter {
    std::vector<char> memory_;
    size_t memory_idx;
    size_t token_idx;
    bool stop_flag;

    Interpreter() : memory_(), memory_idx(), token_idx(), stop_flag() {
        memory_.push_back(0);
    }

    /* outputs state in a hex codes */ 
    void print_state(void) {
        std::print("Memory[{}], Token[{}], Cells:", memory_idx, token_idx);
        for(size_t i = 0; i < memory_.size(); ++i) {
            std::print("[{:02x}]", memory_[i]);
        }
        std::println();
    }

    /* outputs state in a ascii */ 
    void print_ascii(void) {
        for(char ch : memory_) {
            if(std::isprint(ch)) {
                std::print("[{}]", ch);
            } else {
                std::print("[ ]");
            }
        }
        std::println();
    }

    /* Increment the byte at the data pointer by value */
    void increment(size_t value) {
        size_t tmp = memory_[memory_idx] + value;
        if(tmp >= 0xFF) {
            std::cerr << "Memory overflow\n";
            stop_flag = true;
            return;
        }
        memory_[memory_idx] = tmp;
    }

    /* Decrement the byte at the data pointer by value. */
    void decrement(size_t value) {
        size_t tmp = memory_[memory_idx];
        if(tmp < value) {
            std::cerr << "Memory underflow\n";
            stop_flag = true;
            return;
        }
        memory_[memory_idx] = (tmp - value);
    }

    /* Increment the data pointer by value. */
    void move_right(size_t value) {
        memory_idx += value;
        while(memory_idx >= memory_.size()) {
            memory_.push_back(0);
        }
    }

    /* Decrement the data pointer by one */
    void move_left(size_t value) {
        if(value > memory_idx) {
            stop_flag = true;
            std::cerr << "Memory pointer underflow\n";
            return;
        };
        memory_idx -= value;
    }

    /* Output the byte at the data pointer. */
    void output(char value) {
        while(value--) {
            std::print("{}", memory_[memory_idx]);
        }
    }

    /* Accept one byte of input, storing its value in the byte at the data pointer. */
    void input(auto value) {
        if(value) { std::cerr << "value ignored\n"; }
        memory_[memory_idx] = std::cin.get();
    }

    /* Jump to a position inside of a token buffer */
    void jump (std::vector<Tokenizer::Token> &tokens) {
        if(token_idx < tokens.size()) {
            token_idx = tokens[token_idx].data;
        } else {
            std::cerr << "Tried accessing value outside the token buffer\n";
            stop_flag = true;
        }
    }

    void handle_unexpected() {
        std::cerr << "Undexpected symbol\n";
    }

    void interpret(std::vector<Tokenizer::Token> &tokens) {
        token_idx = 0;
        stop_flag = false;
        while(token_idx < tokens.size() && !stop_flag) {
            auto [type, value] = tokens[token_idx];
            switch(type) {
                case Tokenizer::Symbol::inc : increment(value);  break;
                case Tokenizer::Symbol::dec : decrement(value);  break;
                case Tokenizer::Symbol::movr: move_right(value); break;
                case Tokenizer::Symbol::movl: move_left(value);  break;
                case Tokenizer::Symbol::out : output(value);     break;
                case Tokenizer::Symbol::in  : input(value);      break;
                case Tokenizer::Symbol::loop_begin: 
                    if(memory_[memory_idx] == 0) {
                        jump(tokens);
                    }
                    break;
                case Tokenizer::Symbol::loop_end: 
                    if(memory_[memory_idx] != 0) {
                        jump(tokens);
                    }
                    break;
                case Tokenizer::Symbol::bad: [[fallthrough]];
                case Tokenizer::Symbol::numeric: handle_unexpected();
            }
            ++token_idx;
        }
    }
};
