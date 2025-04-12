cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)
project("Yet Another Brainfuck Interpreter")

# EXECUTABLE
add_executable(example1 src/example1.cpp src/classifier.cpp)
add_executable(example2 src/example2.cpp src/classifier.cpp)

