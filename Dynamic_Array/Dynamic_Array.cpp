#include <iostream>
#include "Dynamic_Array.h"

using namespace std;

int main() 
{
    Array<int> arr;

    for (int i = 1; i <= 10; ++i) {
        arr.insert(i);
    }
    for (int i = 0; i < arr.size(); ++i) {
        arr[i] *= 2;
    }

    for (int i = 0; i < arr.size(); ++i) {
        cout << arr[i] << " ";
    }

    cout << "\n";

    return 0;
}