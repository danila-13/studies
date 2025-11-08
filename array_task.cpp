#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cout << "Введите размер массива: ";
    cin >> n;
    
    vector<int> arr(n);
    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    // Находим минимальный и максимальный элементы
    int min_val = *min_element(arr.begin(), arr.end());
    int max_val = *max_element(arr.begin(), arr.end());
    
    cout << "Минимальный элемент: " << min_val << endl;
    cout << "Максимальный элемент: " << max_val << endl;
    
    // Сумма элементов между min и max
    int min_index = find(arr.begin(), arr.end(), min_val) - arr.begin();
    int max_index = find(arr.begin(), arr.end(), max_val) - arr.begin();
    
    if (min_index > max_index) {
        swap(min_index, max_index);
    }
    
    int sum = 0;
    for (int i = min_index + 1; i < max_index; i++) {
        sum += arr[i];
    }
    
    cout << "Сумма элементов между min и max: " << sum << endl;
    
    // Сортировка по возрастанию
    sort(arr.begin(), arr.end());
    cout << "Отсортированный массив: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}
