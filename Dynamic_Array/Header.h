#include <cstdlib>      // Для malloc и free
#include <new>
#include <cassert>      // Для assert
#include <utility>      // Для move и is_move_constructible

using namespace std;

template<typename T>
class Array final
{
private:
    T* indicator;
    int max_size;
    int current_size;
    static constexpr int default_size = 8;

    // Перераспределение памяти
    void resize(int new_size)
    {
        T* new_indicator = static_cast<T*>(malloc(new_size * sizeof(T)));

        if (!new_indicator) {
            throw bad_alloc();
        }

        // Перемещаем или копируем элементы в новую область
        if constexpr (is_move_constructible<T>::value)
        {
            for (int i = 0; i < current_size; ++i)
            {
                new (new_indicator + i) T(move(indicator[i]));
                indicator[i].~T();
            }
        }
        else
        {
            for (int i = 0; i < current_size; ++i)
            {
                new (new_indicator + i) T(indicator[i]);
                indicator[i].~T();
            }
        }

        free(indicator);

        indicator = new_indicator;
        size = new_size;
    }

    // Освобождение всех элементов
    void clear()
    {
        for (int i = 0; i < current_size; ++i) {
            indicator[i].~T();
        }

        current_size = 0;
    }

    class Iterator
    {
    private:
        T* current;
        T* end;

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

public:
    // Конструкторы
    Array() : Array(default_size) {}

    // explicit - не дает сделать неявное преобразование
    explicit Array(int user_size) : size(user_size), current_size(0)
    {
        indicator = static_cast<T*>(malloc(size * sizeof(T)));

        if (!indicator) {
            throw bad_alloc();
        }
    }

    // Конструктор копирования
    Array(const Array& other) : size(other.size), current_size(other.current_size)
    {
        indicator = static_cast<T*>(malloc(size * sizeof(T)));

        if (!indicator) {
            throw bad_alloc();
        }

        for (int i = 0; i < current_size; ++i) {
            new (indicator + i) T(other.indicator[i]);
        }
    }

    // Конструктор перемещения
    Array(Array&& other) noexcept : indicator(other.indicator), size(other.size), current_size(other.current_size)
    {
        other.indicator = nullptr;
        other.size = 0;
        other.current_size = 0;
    }

    // Оператор присваивания копированием
    Array& operator = (const Array& other)
    {
        if (this != &other)
        {
            clear();
            free(indicator);

            size = other.size;
            current_size = other.current_size;

            indicator = static_cast<T*>(malloc(size * sizeof(T)));

            if (!indicator) {

                throw bad_alloc();
            }

            for (int i = 0; i < current_size; ++i) {
                new (indicator + i) T(other.indicator[i]);
            }
        }

        return *this;
    }

    // Оператор присваивания перемещением
    Array& operator = (Array&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            free(indicator);

            indicator = other.indicator;
            size = other.size;
            current_size = other.current_size;

            other.indicator = nullptr;
            other.size = 0;
            other.current_size = 0;
        }

        return *this;
    }

    // Оператор индексирования (константный)
    const T& operator[](int index) const {
        assert(index >= 0 && index < current_size);

        return indicator[index];
    }

    // Оператор индексирования (не константный)
    T& operator[](int index) {
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
        // Механизм утверждений
        assert(index >= 0 && index <= current_size);

        if (current_size >= size) {
            resize(2 * size);
        }

        // Сдвигаем элементы вправо, используя перемещение или копирование
        if constexpr (is_move_constructible<T>::value)
        {
            for (int i = current_size; i > index; --i)
            {
                new (indicator + i) T(move(indicator[i - 1]));
                indicator[i - 1].~T();
            }
        }
        else
        {
            for (int i = current_size; i > index; --i)
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

    // Удаление по индексу
    void remove(int index)
    {
        // Механизм утверждений
        assert(index >= 0 && index < current_size);

        indicator[index].~T();

        // Сдвигаем элементы влево перемещением или копированием
        for (int i = index; i < current_size - 1; ++i)
        {
            if constexpr (is_move_assignable<T>::value) {
                indicator[i] = move(indicator[i + 1]);
            }
            else {
                indicator[i] = indicator[i + 1];
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

    Iterator reverseIterator() {
        return Iterator(indicator + current_size - 1, current_size);
    }

    ConstIterator reverseIterator() const {
        return ConstIterator(indicator + current_size - 1, current_size);
    }
};