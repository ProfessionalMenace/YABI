cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 23)
project("Yet Another Brainfuck Interpreter" CXX)

add_library(classifier)
target_sources(classifier PUBLIC
    FILE_SET CXX_MODULES FILES
      lib/classifier.cppm
)

add_library(interpreter)
target_sources(interpreter PUBLIC
    FILE_SET CXX_MODULES FILES
      lib/interpreter.cppm
      lib/classifier.cppm
)

add_executable(example1 src/example1.cpp)
target_link_libraries(example1 PRIVATE interpreter classifier)

add_executable(example2 src/example2.cpp)
target_link_libraries(example2 PRIVATE interpreter classifier)
