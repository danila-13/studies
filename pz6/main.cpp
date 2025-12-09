#include <iostream>
#include <algorithm>
#include <stdexcept>

class IntArray {
private:
    int* data;
    size_t size;

    bool isValidValue(int value) const {
        return value >= -100 && value <= 100;
    }

public:
    explicit IntArray(size_t sz = 0) : size(sz) {
        data = (size > 0) ? new int[size]() : nullptr;
    }

    ~IntArray() {
        delete[] data;
    }

    IntArray(const IntArray& other) : size(other.size) {
        data = (size > 0) ? new int[size] : nullptr;
        if (data && other.data) {
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    IntArray& operator=(const IntArray& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        data = (size > 0) ? new int[size] : nullptr;
        if (data && other.data) {
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    int get(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void set(size_t index, int value) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        if (!isValidValue(value)) {
            throw std::invalid_argument("Value not in [-100, 100]");
        }
        data[index] = value;
    }

    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i != size - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    size_t getSize() const { return size; }

    void pushBack(int value) {
        if (!isValidValue(value)) {
            throw std::invalid_argument("Value not in [-100, 100]");
        }
        int* newData = new int[size + 1];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        newData[size] = value;
        delete[] data;
        data = newData;
        ++size;
    }

    void add(const IntArray& other) {
        size_t n = std::min(size, other.size);
        for (size_t i = 0; i < n; ++i) {
            int s = data[i] + other.data[i];
            if (s < -100) s = -100;
            else if (s > 100) s = 100;
            data[i] = s;
        }
    }

    void subtract(const IntArray& other) {
        size_t n = std::min(size, other.size);
        for (size_t i = 0; i < n; ++i) {
            int d = data[i] - other.data[i];
            if (d < -100) d = -100;
            else if (d > 100) d = 100;
            data[i] = d;
        }
    }
};

int main() {
    IntArray arr(3);
    try {
        arr.set(0, 10);
        arr.set(1, -50);
        arr.set(2, 100);
        arr.set(0, 200);  // ← исключение
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    try {
        arr.get(5);  // ← исключение
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    arr.print();
    return 0;
}