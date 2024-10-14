#include <iostream>
#include "Dynamic_Array.h"

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
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}