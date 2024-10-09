#include <iostream>
#include <new>        // Для operator new[]
#include <stdexcept>  // Для invalid_argument

using namespace std;

template<typename T>
class Array final 
{

private:
    T* indicator;      
    int size;
    int current_size;
    static const int default_size = 4;

public:

    Array() : Array(default_size) {}

    Array(int size)
    {
        if (size <= 0) {
            throw invalid_argument("Size must be positive.");
        }

        this.size = size;
        current_size = 0;
        indicator = static_cast<T*>(operator new[](size * sizeof(T)));
    }

    ~Array() 
    {
        for (int i = 0; i < current_size; ++i) {
            indicator[i].~T();
        }

        operator delete[](indicator);
    }

    int insert(const T& value);

    int insert(int index, const T& value);

    const T& operator[](int index) const;

    T& operator[](int index);

    int size() const{
        return current_size;
    }
};


int main()
{
    std::cout << "Hello World!\n";
}