#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <iostream>
#include <cmath>
#include <type_traits>
#include <stdexcept>

template <typename T>
class Array {
    std::vector<T> data;

public:
    explicit Array(size_t n = 0) : data(n) {}
    Array(const std::initializer_list<T>& list) : data(list) {}

    size_t size() const { return data.size(); }
    const T& operator[](size_t i) const { return data[i]; }
    T& operator[](size_t i) { return data[i]; }

    void set(size_t i, const T& v) {
        if (i >= data.size()) throw std::out_of_range("Index out of range");
        data[i] = v;
    }

    template <typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, void>::type
    setNumeric(size_t i, const U& v) {
        if (i >= data.size()) throw std::out_of_range("Index out of range");
        if (v < U(-100) || v > U(100))
            throw std::invalid_argument("Value not in [-100, 100]");
        data[i] = v;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Array<T>& a) {
    os << "[";
    for (size_t i = 0; i < a.size(); ++i) {
        if (i) os << ", ";
        os << a[i];
    }
    return os << "]";
}

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, double>::type
distance(const Array<T>& a, const Array<T>& b) {
    if (a.size() != b.size())
        throw std::invalid_argument("Size mismatch");
    double s = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        double d = static_cast<double>(a[i]) - static_cast<double>(b[i]);
        s += d * d;
    }
    return std::sqrt(s);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, double>::type
distance(const Array<T>&, const Array<T>&) {
    throw std::bad_typeid();
}

#endif