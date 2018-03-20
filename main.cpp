#include <iostream>
#include "Logger.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;
    TRACE_DEBUG("hello world");
    return 0;
}