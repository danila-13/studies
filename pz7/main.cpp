#include "Array.h"
#include <iostream>
#include <string>

int main() {
    Array<int> a = {1, 2, 3};
    Array<int> b = {4, 6, 8};

    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";

    try {
        std::cout << "dist(a,b) = " << distance(a, b) << "\n";
        a.setNumeric(0, 50);
        a.setNumeric(1, 200);  // исключение
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    Array<std::string> s = {"x", "y"};
    try {
        distance(s, s);  // std::bad_typeid
    } catch (const std::exception& e) {
        std::cerr << "Non-numeric: " << e.what() << "\n";
    }

    return 0;
}