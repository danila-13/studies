#include <iostream>
#include <algorithm>  

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
            std::cerr << "Error: Index out of bounds (" << index << " >= " << size << ")\n";
            return 0;  
        }
        return data[index];
    }

    
    void set(size_t index, int value) {
        if (index >= size) {
            std::cerr << "Error: Index out of bounds (" << index << " >= " << size << ")\n";
            return;
        }
        if (!isValidValue(value)) {
            std::cerr << "Error: Value " << value << " is not in range [-100, 100]\n";
            return;
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

    
    size_t getSize() const {
        return size;
    }

    
    void pushBack(int value) {
        if (!isValidValue(value)) {
            std::cerr << "Error: Value " << value << " is not in range [-100, 100]\n";
            return;
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
        size_t common = std::min(size, other.size);
        for (size_t i = 0; i < common; ++i) {
            int sum = data[i] + other.data[i];
            
            if (!isValidValue(sum)) {
                std::cerr << "Warning: sum at index " << i << " (" << data[i] << " + " << other.data[i]
                          << " = " << sum << ") is out of range [-100, 100]; clamped.\n";
                sum = (sum < -100) ? -100 : 100;
            }
            data[i] = sum;
        }
        
    }

   
    void subtract(const IntArray& other) {
        size_t common = std::min(size, other.size);
        for (size_t i = 0; i < common; ++i) {
            int diff = data[i] - other.data[i];
            if (!isValidValue(diff)) {
                std::cerr << "Warning: difference at index " << i << " (" << data[i] << " - " << other.data[i]
                          << " = " << diff << ") is out of range [-100, 100]; clamped.\n";
                diff = (diff < -100) ? -100 : 100;
            }
            data[i] = diff;
        }
        
    }
};

int main() {
    IntArray arr1(3);
    arr1.set(0, 10);
    arr1.set(1, -50);
    arr1.set(2, 100);
    std::cout << "arr1: ";
    arr1.print();  

    IntArray arr2(2);
    arr2.set(0, 5);
    arr2.set(1, -10);
    std::cout << "arr2: ";
    arr2.print();  

    arr1.add(arr2);
    std::cout << "After arr1.add(arr2): ";
    arr1.print();  

    arr1.pushBack(-99);
    std::cout << "After pushBack(-99): ";
    arr1.print();  

    IntArray arr3 = arr1;  
    std::cout << "arr3 (copy of arr1): ";
    arr3.print();

    arr3.subtract(arr1);
    std::cout << "After arr3.subtract(arr1): ";
    arr3.print();  

    arr1.set(10, 5);  
    arr1.set(0, 200);  

    return 0;
}