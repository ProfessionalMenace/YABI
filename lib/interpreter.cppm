module;
import classifier;
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>
export module interpreter;

export class Interpreter {
    size_t memory_size;
    size_t state_idx;
    uint8_t *memory_state;
    bool stop_flag;
    bool cycle_value;
    bool cycle_memory;

    void increment(uint8_t value);
    void decrement(uint8_t value);

    void move_right(uint8_t value);
    void move_left(uint8_t value);

    void output(uint8_t value);
    void input(uint8_t value);

    void handle_unexpected();

  public:
    Interpreter(size_t size, size_t offset, bool cycle_value, bool cycle_memory);
    ~Interpreter();

    void interpret(const std::vector<Token> &tokens);
    void print_state(void);
};

Interpreter::Interpreter(size_t size, size_t offset = 0, bool cycle_value = true, bool cycle_memory = true)
    : memory_size(size), memory_state(nullptr), state_idx(offset), stop_flag(false), cycle_value(cycle_value),
      cycle_memory(cycle_memory) {
    // add check here
    memory_state = new uint8_t[size]{};
}

Interpreter::~Interpreter() { delete[] memory_state; }

/* outputs state in a ascii */
void Interpreter::print_state(void) {
    for (size_t i = 0; i < memory_size; ++i) {
        char ch = memory_state[i];
        if (std::isprint(ch)) {
            std::print("[{:2}]", ch);
        } else {
            std::print("[{:2x}]", ch);
        }
    }
    std::println();
}

/* Increment the byte at the data pointer by value */
void Interpreter::increment(uint8_t value) {
    size_t tmp = memory_state[state_idx] + value;
    if (!cycle_value && tmp >= 0xFF) {
        std::println(std::cerr, "Value overflow at {}", state_idx);
        print_state();
        stop_flag = true;
        return;
    }
    memory_state[state_idx] = tmp;
}

/* Decrement the byte at the data pointer by value. */
void Interpreter::decrement(uint8_t value) {
    if (!cycle_value && memory_state[state_idx] < value) {
        std::println(std::cerr, "Value underflow at {}", state_idx);
        print_state();
        stop_flag = true;
        return;
    }
    memory_state[state_idx] = memory_state[state_idx] - value;
}

/* Increment the data pointer by value. */
void Interpreter::move_right(uint8_t value) {
    size_t tmp = state_idx + value;
    if (!cycle_memory && tmp > memory_size) {
        std::println(std::cerr, "Memory pointer overflow");
        print_state();
        stop_flag = true;
        return;
    }
    state_idx = tmp % memory_size;
}

/* Decrement the data pointer by one */
void Interpreter::move_left(uint8_t value) {
    size_t tmp = memory_size + state_idx - value;
    if (!cycle_memory && tmp < memory_size) {
        std::println(std::cerr, "Memory pointer underflow");
        print_state();
        stop_flag = true;
        return;
    }
    state_idx = tmp % memory_size;
}

/* Output the byte at the data pointer. */
void Interpreter::output(uint8_t value) {
    while (value--)
        std::cout << static_cast<char>(memory_state[state_idx]);
}

/* Accept one byte of input, storing its value in the byte at the data pointer. */
void Interpreter::input(uint8_t value) { memory_state[state_idx] = std::cin.get(); }

void Interpreter::handle_unexpected() {
    std::println(std::cerr, "Undexpected symbol at {}", state_idx);
    print_state();
    stop_flag = true;
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
            if (memory_state[state_idx] == 0) {
                jump(value);
            }
            break;
        case Symbol::loop_end:
            if (memory_state[state_idx] != 0) {
                jump(value);
            }
            break;
        case Symbol::bad: handle_unexpected();
        }
        ++token_idx;
    }
}
