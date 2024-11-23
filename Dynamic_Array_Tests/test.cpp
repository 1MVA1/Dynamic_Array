#include "pch.h"
#include <string>
#include <gtest/gtest.h>
#include "../Dynamic_Array/Dynamic_array.h"

/*
    ASSERT_EQ(expected, actual) — проверяет равенство.
    ASSERT_NE(expected, actual) — проверяет, что значения разные.
    ASSERT_TRUE(expression) — проверяет, что выражение истинно.
    ASSERT_FALSE(expression) — проверяет, что выражение ложно.
    EXPECT_* — аналогичны ASSERT_*, но позволяют тесту продолжить выполнение, даже если одно из утверждений не выполнено.
*/

using namespace std;

// Тест конструктора по умолчанию
TEST(Constructor) 
{
    Array<int> arr;

    ASSERT_EQ(arr.size(), 0);
}

// Тест конструктора с параметром
TEST(Constructor_with_param) 
{
    Array<int> arr(5);

    ASSERT_EQ(arr.size(), 0);
}

// Тест вставки элемента
TEST(Insert) 
{
    Array<int> arr;

    arr.insert(10);

    ASSERT_EQ(arr.size(), 1);

    ASSERT_EQ(arr[0], 10);
}

// Тест вставки по индексу
TEST(Insert_with_index) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);
    arr.insert(1, 15);  // Вставляем 15 на индекс 1

    ASSERT_EQ(arr.size(), 3);

    ASSERT_EQ(arr[0], 10);
    ASSERT_EQ(arr[1], 15);
    ASSERT_EQ(arr[2], 20);
}

// Тест удаления элемента
TEST(Remove) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    arr.remove(1);  // Удаляем элемент 20

    ASSERT_EQ(arr.size(), 2);

    ASSERT_EQ(arr[0], 10);
    ASSERT_EQ(arr[1], 30);
}

// Тест копирующего конструктора
TEST(Copy_constructor) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);

    Array<int> arrCopy(arr);  // Копируем массив

    ASSERT_EQ(arrCopy.size(), 2);

    ASSERT_EQ(arrCopy[0], 10);
    ASSERT_EQ(arrCopy[1], 20);
}

// Тест конструктора перемещения
TEST(Move_constructor) 
{
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);

    Array<int> arrMove(move(arr));  // Перемещаем массив

    ASSERT_EQ(arrMove.size(), 2);

    ASSERT_EQ(arrMove[0], 10);
    ASSERT_EQ(arrMove[1], 20);

    ASSERT_EQ(arr.size(), 0);  // Исходный массив должен быть пустым
}

// Тест операторов присваивания 1
TEST(Copy_assignment_operator) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);

    Array<int> arrCopy;
    arrCopy = arr;  // Копируем через оператор присваивания

    ASSERT_EQ(arrCopy.size(), 2);

    ASSERT_EQ(arrCopy[0], 10);
    ASSERT_EQ(arrCopy[1], 20);
}

// Тест операторов присваивания 2
TEST(Move_assignment_operator) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);

    Array<int> arrMove;
    arrMove = move(arr);  // Перемещаем через оператор присваивания

    ASSERT_EQ(arrMove.size(), 2);

    ASSERT_EQ(arrMove[0], 10);
    ASSERT_EQ(arrMove[1], 20);

    ASSERT_EQ(arr.size(), 0);  // Исходный массив должен быть пустым
}

// Тест для итераторов
TEST(Iterators) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    auto it = arr.iterator();

    ASSERT_TRUE(it.hasNext());

    ASSERT_EQ(it.get(), 10);

    it.next();
    ASSERT_EQ(it.get(), 20);

    it.next();
    ASSERT_EQ(it.get(), 30);

    it.next();
    ASSERT_FALSE(it.hasNext());
}

// Тест для реверс итераторов
TEST(Reverse_iterators)
{
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    auto rit = arr.reverseIterator();

    ASSERT_TRUE(rit.hasNext());
    ASSERT_EQ(rit.get(), 30);

    rit.next();
    ASSERT_EQ(rit.get(), 20);

    rit.next();
    ASSERT_EQ(rit.get(), 10);

    rit.next();
    ASSERT_FALSE(rit.hasNext());
}

// Тест конструктора по умолчанию
TEST(String_Constructor)
{
    Array<string> arr;

    ASSERT_EQ(arr.size(), 0);
}

// Тест конструктора с параметром
TEST(String_Constructor_with_param)
{
    Array<string> arr(5);

    ASSERT_EQ(arr.size(), 0);
}

// Тест вставки элемента
TEST(String_Insert)
{
    Array<string> arr;

    arr.insert("Aa");

    ASSERT_EQ(arr.size(), 1);

    ASSERT_EQ(arr[0], "Aa");
}

// Тест вставки по индексу
TEST(String_Insert_with_index)
{
    Array<string> arr;

    arr.insert("Aa");
    arr.insert("Cc");
    arr.insert(1, "Bb"); 

    ASSERT_EQ(arr.size(), 3);

    ASSERT_EQ(arr[0], "Aa");
    ASSERT_EQ(arr[1], "Bb");
    ASSERT_EQ(arr[2], "Cc");
}

// Тест удаления элемента
TEST(String_Remove)
{
    Array<string> arr;

    arr.insert("Aa");
    arr.insert("Bb");
    arr.insert("Cc");

    arr.remove(1);  // Удаляем элемент "Bb"

    ASSERT_EQ(arr.size(), 2);

    ASSERT_EQ(arr[0], "Aa");
    ASSERT_EQ(arr[1], "Cc");
}

// Тест копирующего конструктора
TEST(String_Copy_constructor)
{
    Array<string> arr;

    arr.insert("Aa");
    arr.insert("Bb");

    Array<string> arrCopy(arr);  // Копируем массив

    ASSERT_EQ(arrCopy.size(), 2);

    ASSERT_EQ(arrCopy[0], "Aa");
    ASSERT_EQ(arrCopy[1], "Bb");
}

// Тест конструктора перемещения
TEST(String_Move_constructor)
{
    Array<string> arr;
    arr.insert("Aa");
    arr.insert("Bb");

    Array<string> arrMove(move(arr));  // Перемещаем массив

    ASSERT_EQ(arrMove.size(), 2);

    ASSERT_EQ(arrMove[0], "Aa");
    ASSERT_EQ(arrMove[1], "Bb");

    ASSERT_EQ(arr.size(), 0);  // Исходный массив должен быть пустым
}

// Тест операторов присваивания 1
TEST(String_Copy_assignment_operator)
{
    Array<string> arr;

    arr.insert("Aa");
    arr.insert("Bb");

    Array<string> arrCopy;
    arrCopy = arr;  // Копируем через оператор присваивания

    ASSERT_EQ(arrCopy.size(), 2);

    ASSERT_EQ(arrCopy[0], "Aa");
    ASSERT_EQ(arrCopy[1], "Bb");
}

// Тест операторов присваивания 2
TEST(String_Move_assignment_operator)
{
    Array<string> arr;

    arr.insert("Aa");
    arr.insert("Bb");

    Array<string> arrMove;
    arrMove = move(arr);  // Перемещаем через оператор присваивания

    ASSERT_EQ(arrMove.size(), 2);

    ASSERT_EQ(arrMove[0], "Aa");
    ASSERT_EQ(arrMove[1], "Bb");

    ASSERT_EQ(arr.size(), 0);  // Исходный массив должен быть пустым
}

// Тест для итераторов
TEST(String_Iterators)
{
    Array<string> arr;

    arr.insert("Aa");
    arr.insert("Bb");
    arr.insert("Cc");

    auto it = arr.iterator();

    ASSERT_TRUE(it.hasNext());

    ASSERT_EQ(it.get(), "Aa");

    it.next();
    ASSERT_EQ(it.get(), "Bb");

    it.next();
    ASSERT_EQ(it.get(), "Cc");

    it.next();
    ASSERT_FALSE(it.hasNext());
}

// Тест для реверс итераторов
TEST(String_Reverse_iterators)
{
    Array<string> arr;
    arr.insert("Aa");
    arr.insert("Bb");
    arr.insert("Cc");

    auto rit = arr.reverseIterator();

    ASSERT_TRUE(rit.hasNext());
    ASSERT_EQ(rit.get(), "Cc");

    rit.next();
    ASSERT_EQ(rit.get(), "Bb");

    rit.next();
    ASSERT_EQ(rit.get(), "Aa");

    rit.next();
    ASSERT_FALSE(rit.hasNext());
}