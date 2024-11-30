#pragma once

#include <cstdlib>      // Для malloc и free
#include <new>          // Для оператора new с размещением
#include <cassert>      // Для assert
#include <utility>      // Для move и is_move_constructible
#include <algorithm>    // Для swap

using namespace std;

template<typename T>
class Array final
{
private:
    T* indicator;                               // Указатель на массив элементов
    int max_size;                               // Максимальный размер массива
    int current_size;                           // Текущий размер массива
    static constexpr int default_size = 8;      // Размер массива по умолчанию

    // Перераспределение памяти при увеличении массива
    void resize(int new_size)
    {
        T* new_indicator = static_cast<T*>(malloc(new_size * sizeof(T))); // Выделение новой памяти

        if (!new_indicator) {
            throw bad_alloc(); // Исключение в случае нехватки памяти
        }

        // Перемещаем или копируем элементы в новую область
        if constexpr (is_move_constructible<T>)
        {
            for (int i = 0; i < current_size; ++i)
            {
                new (new_indicator + i) T(move(indicator[i])); // Перемещающий конструктор
                //После move  старый объект остаётся в "валидном, но неопределённом состоянии". Для завершения работы с ним его нужно разрушить вручную
                indicator[i].~T();
            }
        }
        else
        {
            for (int i = 0; i < current_size; ++i)
            {
                new (new_indicator + i) T(indicator[i]); // Копирующий конструктор
                indicator[i].~T();
            }
        }

        free(indicator); // Освобождаем старую память

        indicator = new_indicator;
        max_size = new_size;
    }

    // Освобождение всех элементов
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
        T* current;      // Текущий элемент
        T* end;          // Конечный элемент

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
    // Конструкторы
    Array() : Array(default_size) {}

    // explicit - не дает сделать неявное преобразование
    explicit Array(int user_size) : max_size(user_size), current_size(0)
    {
        indicator = static_cast<T*>(malloc(max_size * sizeof(T)));

        if (!indicator) {
            throw bad_alloc();
        }
    }

    // Конструктор копирования
    Array(const Array& other) : max_size(other.max_size), current_size(other.current_size)
    {
        indicator = static_cast<T*>(malloc(max_size * sizeof(T)));

        if (!indicator) {
            throw bad_alloc();
        }

        // Копируем элементы массива
        for (int i = 0; i < current_size; ++i) {
            new (indicator + i) T(other.indicator[i]);
        }
    }

    // Конструктор перемещения
    Array(Array&& other) noexcept : indicator(other.indicator), max_size(other.max_size), current_size(other.current_size)
    {
        // Очищаем данные исходного массива
        other.indicator = nullptr;
        other.max_size = 0;
        other.current_size = 0;
    }

    // Оператор присваивания копированием
    Array& operator=(const Array& other) 
    {
        // Создаем временную копию other
        Array temp(other);

        // Если что - то пошло не так, текущий объект остается неизменным, так как изменения применяются только после обмена
        // swap - это просто обмен указателей и размеров
        swap_(temp);

        return *this;
    }

    // Оператор присваивания перемещением
    Array& operator=(Array&& other) noexcept 
    {
        // Используем swap напрямую с rvalue-ссылкой
        swap_(other);

        return *this;
    }

    // Оператор индексирования (константный)
    const T& operator[](int index) const 
    {
        // Механизм утверждений
        assert(index >= 0 && index < current_size); // Проверка на допустимый индекс

        return indicator[index];
    }

    // Оператор индексирования (не константный)
    T& operator[](int index) 
    {
        assert(index >= 0 && index < current_size);

        return indicator[index];
    }

    // Деструктор
    ~Array()
    {
        clear();
        free(indicator);
    }

    // Получение размера
    int size() const {
        return current_size;
    }

    // Вставка в конец
    int insert(const T& value) {
        return insert(current_size, value);
    }

    // Вставка по индексу
    int insert(int index, const T& value)
    {
        assert(index >= 0 && index <= current_size);

        if (current_size >= max_size)
        {
            resize(2 * max_size);
        }

        // Сдвигаем элементы вправо, начиная с конца
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

        // Вставляем новый элемент
        new (indicator + index) T(value);
        ++current_size;

        return index;
    }

    //placement new ?
    // Удаление по индексу
    void remove1(int index)
    {
        assert(index >= 0 && index < current_size);

        if constexpr (is_move_constructible_v<T>)
        {
            // Сдвигаем элементы влево
            for (int i = index; i < current_size - 1; ++i) {
                indicator[i].operator=(move(indicator[i + 1]));
            }  

            indicator[current_size - 1].~T();
        }
        else 
        {
            // Элемент, который будет перезаписан, уже не используется
            for (int i = index; i < current_size - 1; ++i) {
                indicator[i] = indicator[i + 1];
            }

            indicator[current_size - 1].~T();
        }

        --current_size;
    }

    // Почему placement new
    //      1. Без этого и явного вызова деструктора не получится корректно обработать ресурсы, которыми управляет объект
    //      2. Для типов, поддерживающих перемещение, позволяет избежать лишнего копирования. Вместо копирования данных вызывается конструктор перемещения
    void remove(int index)
    {
        assert(index >= 0 && index < current_size);

        // Уничтожаем удаляемый элемент
        indicator[index].~T();

        if constexpr (is_move_constructible_v<T>)
        {
            // Сдвигаем оставшиеся элементы влево
            for (int i = index; i < current_size - 1; ++i)
            {
                new (indicator + i) T(move(indicator[i + 1]));  // Конструируем новый объект на месте
                indicator[i + 1].~T();                          // Уничтожаем старый объект
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

    // Итераторы
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