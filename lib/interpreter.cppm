module;
import classifier;
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>
export module interpreter;

export class Interpreter {
    std::vector<uint8_t> interpreter_state;
    size_t state_idx;
    bool stop_flag;
    bool cycle;

    void print_state(void);
    void print_ascii(void);

    void increment(uint8_t value);
    void decrement(uint8_t value);

    void move_right(uint8_t value);
    void move_left(uint8_t value);

    void output(uint8_t value);
    void input(uint8_t value);

    void handle_unexpected();

  public:
    Interpreter();
    void interpret(const std::vector<Token> &tokens);
};

Interpreter::Interpreter() : interpreter_state(1, 0), state_idx(0), stop_flag(false), cycle(true) {}

/* outputs state in a hex codes */
void Interpreter::print_state(void) { std::println(std::cerr, "Don't print state!!!"); }

/* outputs state in a ascii */
void Interpreter::print_ascii(void) {
    for (char ch : interpreter_state) {
        if (std::isprint(ch)) {
            std::print("[{}]", ch);
        } else {
            std::print("[{:2x}]", ch);
        }
    }
    std::println();
}

/* Increment the byte at the data pointer by value */
void Interpreter::increment(uint8_t value) {
    size_t tmp = interpreter_state[state_idx] + value;
    if (!cycle && tmp >= 0xFF) {
        std::println(std::cerr, "Memory overflow at {}", state_idx);
        print_ascii();
        stop_flag = true;
        return;
    }
    interpreter_state[state_idx] = tmp;
}

/* Decrement the byte at the data pointer by value. */
void Interpreter::decrement(uint8_t value) {
    size_t tmp = interpreter_state[state_idx];
    if (!cycle && tmp < value) {
        std::println(std::cerr, "Memory underflow at {}", state_idx);
        print_ascii();
        stop_flag = true;
        return;
    }
    interpreter_state[state_idx] = (tmp - value);
}

/* Increment the data pointer by value. */
void Interpreter::move_right(uint8_t value) {
    state_idx += value;
    while (state_idx >= interpreter_state.size()) {
        interpreter_state.push_back(0);
    }
}

/* Decrement the data pointer by one */
void Interpreter::move_left(uint8_t value) {
    if (value > state_idx) {
        stop_flag = true;
        std::cerr << "Memory pointer underflow\n";
        print_ascii();
        return;
    };
    state_idx -= value;
}

/* Output the byte at the data pointer. */
void Interpreter::output(uint8_t value) {
    while (value--) {
        std::print("{}", static_cast<char>(interpreter_state[state_idx]));
    }
}

/* Accept one byte of input, storing its value in the byte at the data pointer. */
void Interpreter::input(uint8_t value) {
    while (value--) {
        interpreter_state[state_idx] = std::cin.get();
    }
}

void Interpreter::handle_unexpected() {
    std::println(std::cerr, "Undexpected symbol at {}", state_idx);
    print_ascii();
}

void Interpreter::interpret(const std::vector<Token> &tokens) {
    size_t token_idx = 0;

    /* Jump to a position inside of a token buffer */
    auto jump = [this, &token_idx, &tokens](uint8_t value) {
        if (token_idx < tokens.size()) {
            token_idx = value;
        } else {
            std::println(std::cerr, "Tried accessing value outside the token buffer at {}", token_idx);
            stop_flag = true;
        }
    };

    while (token_idx < tokens.size() && !stop_flag) {
        const auto &[type, value] = tokens[token_idx];
        switch (type) {
        case Symbol::inc: increment(value); break;
        case Symbol::dec: decrement(value); break;
        case Symbol::movr: move_right(value); break;
        case Symbol::movl: move_left(value); break;
        case Symbol::out: output(value); break;
        case Symbol::in: input(value); break;
        case Symbol::loop_begin:
            if (interpreter_state[state_idx] == 0) {
                jump(value);
            }
            break;
        case Symbol::loop_end:
            if (interpreter_state[state_idx] != 0) {
                jump(value);
            }
            break;
        case Symbol::bad: handle_unexpected();
        }
        ++token_idx;
    }
}
