#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    int min_val = *min_element(arr.begin(), arr.end());
    int max_val = *max_element(arr.begin(), arr.end());
    
    int min_index = find(arr.begin(), arr.end(), min_val) - arr.begin();
    int max_index = find(arr.begin(), arr.end(), max_val) - arr.begin();
    
    if (min_index > max_index) swap(min_index, max_index);
    
    int sum = 0;
    for (int i = min_index + 1; i < max_index; i++) {
        sum += arr[i];
    }
    
    sort(arr.begin(), arr.end());
    
    cout << sum << endl;
    for (int num : arr) {
        cout << num << " ";
    }
    
    return 0;
}
