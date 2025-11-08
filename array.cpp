#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Array {
protected:
    int* data;
    int size;

public:
    Array(int n) : size(n) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    Array(const Array& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    ~Array() {
        delete[] data;
    }

    void setValue(int index, int value) {
        if (index < 0 || index >= size) return;
        if (value < -100 || value > 100) return;
        data[index] = value;
    }

    int getValue(int index) {
        if (index < 0 || index >= size) return 0;
        return data[index];
    }

    void print() {
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    void pushBack(int value) {
        if (value < -100 || value > 100) return;

        int* newData = new int[size + 1];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        newData[size] = value;
        
        delete[] data;
        data = newData;
        size++;
    }

    Array add(const Array& other) {
        Array result(max(size, other.size));
        
        for (int i = 0; i < result.size; i++) {
            int val1 = (i < size) ? data[i] : 0;
            int val2 = (i < other.size) ? other.data[i] : 0;
            result.data[i] = val1 + val2;
        }
        return result;
    }

    Array subtract(const Array& other) {
        Array result(max(size, other.size));
        
        for (int i = 0; i < result.size; i++) {
            int val1 = (i < size) ? data[i] : 0;
            int val2 = (i < other.size) ? other.data[i] : 0;
            result.data[i] = val1 - val2;
        }
        return result;
    }

    int getSize() const { return size; }
    int* getData() const { return data; }
};

class ExtendedArray : public Array {
public:
    ExtendedArray(int n) : Array(n) {}
    ExtendedArray(const Array& other) : Array(other) {}

    double getAverage() {
        if (size == 0) return 0;
        double sum = 0;
        for (int i = 0; i < size; i++) {
            sum += data[i];
        }
        return sum / size;
    }

    double getMedian() {
        if (size == 0) return 0;
        vector<int> sorted(data, data + size);
        sort(sorted.begin(), sorted.end());
        if (size % 2 == 1) {
            return sorted[size / 2];
        } else {
            return (sorted[size / 2 - 1] + sorted[size / 2]) / 2.0;
        }
    }

    int getMin() {
        if (size == 0) return 0;
        int minVal = data[0];
        for (int i = 1; i < size; i++) {
            if (data[i] < minVal) minVal = data[i];
        }
        return minVal;
    }

    int getMax() {
        if (size == 0) return 0;
        int maxVal = data[0];
        for (int i = 1; i < size; i++) {
            if (data[i] > maxVal) maxVal = data[i];
        }
        return maxVal;
    }
};

int main() {
    ExtendedArray arr(5);
    
    arr.setValue(0, 10);
    arr.setValue(1, 20);
    arr.setValue(2, 30);
    arr.setValue(3, 40);
    arr.setValue(4, 50);
    
    cout << "Массив: ";
    arr.print();
    
    cout << "Среднее: " << arr.getAverage() << endl;
    cout << "Медиана: " << arr.getMedian() << endl;
    cout << "Минимум: " << arr.getMin() << endl;
    cout << "Максимум: " << arr.getMax() << endl;
    
    return 0;
}
