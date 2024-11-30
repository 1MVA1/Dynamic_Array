#pragma once

#include <cstdlib>      // ��� malloc � free
#include <new>          // ��� ��������� new � �����������
#include <cassert>      // ��� assert
#include <utility>      // ��� move � is_move_constructible
#include <algorithm>    // ��� swap

using namespace std;

template<typename T>
class Array final
{
private:
    T* indicator;                               // ��������� �� ������ ���������
    int max_size;                               // ������������ ������ �������
    int current_size;                           // ������� ������ �������
    static constexpr int default_size = 8;      // ������ ������� �� ���������

    // ����������������� ������ ��� ���������� �������
    void resize(int new_size)
    {
        T* new_indicator = static_cast<T*>(malloc(new_size * sizeof(T))); // ��������� ����� ������

        if (!new_indicator) {
            throw bad_alloc(); // ���������� � ������ �������� ������
        }

        // ���������� ��� �������� �������� � ����� �������
        if constexpr (is_move_constructible<T>)
        {
            for (int i = 0; i < current_size; ++i)
            {
                new (new_indicator + i) T(move(indicator[i])); // ������������ �����������
                //����� move  ������ ������ ������� � "��������, �� ������������� ���������". ��� ���������� ������ � ��� ��� ����� ��������� �������
                indicator[i].~T();
            }
        }
        else
        {
            for (int i = 0; i < current_size; ++i)
            {
                new (new_indicator + i) T(indicator[i]); // ���������� �����������
                indicator[i].~T();
            }
        }

        free(indicator); // ����������� ������ ������

        indicator = new_indicator;
        max_size = new_size;
    }

    // ������������ ���� ���������
    void clear()
    {
        for (int i = 0; i < current_size; ++i) {
            indicator[i].~T();
        }

        current_size = 0;
    }

    void swap_(Array& other) noexcept 
    {
        swap(indicator, other.indicator);
        swap(max_size, other.max_size);
        swap(current_size, other.current_size);
    }

    class Iterator
    {
    private:
        T* current;      // ������� �������
        T* end;          // �������� �������

    public:
        Iterator(T* ptr, int size) : current(ptr), end(ptr + size) {}

        const T& get() const {
            return *current;
        }

        void set(const T& value) {
            *current = value;
        }

        void next()
        {
            if (current < end) {
                ++current;
            }
        }

        bool hasNext() const {
            return current < end;
        }
    };

    class ConstIterator
    {
    private:
        const T* current;
        const T* end;

    public:
        ConstIterator(const T* ptr, int size) : current(ptr), end(ptr + size) {}

        const T& get() const {
            return *current;
        }

        void next()
        {
            if (current < end) {
                ++current;
            }
        }

        bool hasNext() const {
            return current < end;
        }
    };

    class ReverseIterator
    {
    private:
        T* current; 
        T* start;   

    public:
        ReverseIterator(T* ptr, int size) : current(ptr + size - 1), start(ptr) {}

        const T& get() const {
            return *current;
        }

        void set(const T& value) {
            *current = value;
        }

        void next() {
            if (current >= start) {
                --current;
            }
        }

        bool hasNext() const {
            return current >= start;
        }
    };

    class ConstReverseIterator
    {
    private:
        const T* current;
        const T* start;   

    public:
        ConstReverseIterator(const T* ptr, int size) : current(ptr + size - 1), start(ptr) {}

        const T& get() const {
            return *current;
        }

        void next() {
            if (current >= start) {
                --current;
            }
        }

        bool hasNext() const {
            return current >= start;
        }
    };

public:
    // ������������
    Array() : Array(default_size) {}

    // explicit - �� ���� ������� ������� ��������������
    explicit Array(int user_size) : max_size(user_size), current_size(0)
    {
        indicator = static_cast<T*>(malloc(max_size * sizeof(T)));

        if (!indicator) {
            throw bad_alloc();
        }
    }

    // ����������� �����������
    Array(const Array& other) : max_size(other.max_size), current_size(other.current_size)
    {
        indicator = static_cast<T*>(malloc(max_size * sizeof(T)));

        if (!indicator) {
            throw bad_alloc();
        }

        // �������� �������� �������
        for (int i = 0; i < current_size; ++i) {
            new (indicator + i) T(other.indicator[i]);
        }
    }

    // ����������� �����������
    Array(Array&& other) noexcept : indicator(other.indicator), max_size(other.max_size), current_size(other.current_size)
    {
        // ������� ������ ��������� �������
        other.indicator = nullptr;
        other.max_size = 0;
        other.current_size = 0;
    }

    // �������� ������������ ������������
    Array& operator=(const Array& other) 
    {
        // ������� ��������� ����� other
        Array temp(other);

        // ���� ��� - �� ����� �� ���, ������� ������ �������� ����������, ��� ��� ��������� ����������� ������ ����� ������
        // swap - ��� ������ ����� ���������� � ��������
        swap_(temp);

        return *this;
    }

    // �������� ������������ ������������
    Array& operator=(Array&& other) noexcept 
    {
        // ���������� swap �������� � rvalue-�������
        swap_(other);

        return *this;
    }

    // �������� �������������� (�����������)
    const T& operator[](int index) const 
    {
        // �������� �����������
        assert(index >= 0 && index < current_size); // �������� �� ���������� ������

        return indicator[index];
    }

    // �������� �������������� (�� �����������)
    T& operator[](int index) 
    {
        assert(index >= 0 && index < current_size);

        return indicator[index];
    }

    // ����������
    ~Array()
    {
        clear();
        free(indicator);
    }

    // ��������� �������
    int size() const {
        return current_size;
    }

    // ������� � �����
    int insert(const T& value) {
        return insert(current_size, value);
    }

    // ������� �� �������
    int insert(int index, const T& value)
    {
        assert(index >= 0 && index <= current_size);

        if (current_size >= max_size)
        {
            resize(2 * max_size);
        }

        // �������� �������� ������, ������� � �����
        for (int i = current_size; i > index; --i)
        {
            if constexpr (is_move_constructible_v<T>)
            {
                new (indicator + i) T(move(indicator[i - 1]));
            }
            else
            {
                new (indicator + i) T(indicator[i - 1]);
                indicator[i - 1].~T(); 
            }
        }

        // ��������� ����� �������
        new (indicator + index) T(value);
        ++current_size;

        return index;
    }

    //placement new ?
    // �������� �� �������
    void remove1(int index)
    {
        assert(index >= 0 && index < current_size);

        if constexpr (is_move_constructible_v<T>)
        {
            // �������� �������� �����
            for (int i = index; i < current_size - 1; ++i) {
                indicator[i].operator=(move(indicator[i + 1]));
            }  

            indicator[current_size - 1].~T();
        }
        else 
        {
            // �������, ������� ����� �����������, ��� �� ������������
            for (int i = index; i < current_size - 1; ++i) {
                indicator[i] = indicator[i + 1];
            }

            indicator[current_size - 1].~T();
        }

        --current_size;
    }

    // ������ placement new
    //      1. ��� ����� � ������ ������ ����������� �� ��������� ��������� ���������� �������, �������� ��������� ������
    //      2. ��� �����, �������������� �����������, ��������� �������� ������� �����������. ������ ����������� ������ ���������� ����������� �����������
    void remove(int index)
    {
        assert(index >= 0 && index < current_size);

        // ���������� ��������� �������
        indicator[index].~T();

        if constexpr (is_move_constructible_v<T>)
        {
            // �������� ���������� �������� �����
            for (int i = index; i < current_size - 1; ++i)
            {
                new (indicator + i) T(move(indicator[i + 1]));  // ������������ ����� ������ �� �����
                indicator[i + 1].~T();                          // ���������� ������ ������
            }
        }
        else
        {
            for (int i = index; i < current_size - 1; ++i)
            {
                new (indicator + i) T(indicator[i + 1]); 
                indicator[i + 1].~T(); 
            }
        }

        --current_size;
    }

    // ���������
    Iterator iterator() {
        return Iterator(indicator, current_size);
    }

    ConstIterator iterator() const {
        return ConstIterator(indicator, current_size);
    }

    ReverseIterator reverseIterator() {
        return ReverseIterator(indicator, current_size);
    }

    ConstReverseIterator reverseIterator() const {
        return ConstReverseIterator(indicator, current_size);
    }
};