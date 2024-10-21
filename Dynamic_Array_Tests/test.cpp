#include "pch.h"
#include <gtest/gtest.h>
#include "../Dynamic_Array/Dynamic_array.h"

/*
    ASSERT_EQ(expected, actual) � ��������� ���������.
    ASSERT_NE(expected, actual) � ���������, ��� �������� ������.
    ASSERT_TRUE(expression) � ���������, ��� ��������� �������.
    ASSERT_FALSE(expression) � ���������, ��� ��������� �����.
    EXPECT_* � ���������� ASSERT_*, �� ��������� ����� ���������� ����������, ���� ���� ���� �� ����������� �� ���������.
*/

using namespace std;

// ���� ������������ �� ���������
TEST(ArrayTest, DefaultConstructor) 
{
    Array<int> arr;

    ASSERT_EQ(arr.size(), 0);
}

// ���� ������������ � ����������
TEST(ArrayTest, ParamConstructor) 
{
    Array<int> arr(5);

    ASSERT_EQ(arr.size(), 0);
}

// ���� ������� ��������
TEST(ArrayTest, InsertElement) 
{
    Array<int> arr;

    arr.insert(10);

    ASSERT_EQ(arr.size(), 1);

    ASSERT_EQ(arr[0], 10);
}

// ���� ������� �� �������
TEST(ArrayTest, InsertElementAtIndex) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);
    arr.insert(1, 15);  // ��������� 15 �� ������ 1

    ASSERT_EQ(arr.size(), 3);

    ASSERT_EQ(arr[0], 10);
    ASSERT_EQ(arr[1], 15);
    ASSERT_EQ(arr[2], 20);
}

// ���� �������� ��������
TEST(ArrayTest, RemoveElement) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    arr.remove(1);  // ������� ������� 20

    ASSERT_EQ(arr.size(), 2);

    ASSERT_EQ(arr[0], 10);
    ASSERT_EQ(arr[1], 30);
}

// ���� ����������� ������������
TEST(ArrayTest, CopyConstructor) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);

    Array<int> arrCopy(arr);  // �������� ������

    ASSERT_EQ(arrCopy.size(), 2);

    ASSERT_EQ(arrCopy[0], 10);
    ASSERT_EQ(arrCopy[1], 20);
}

// ���� ������������ �����������
TEST(ArrayTest, MoveConstructor) 
{
    Array<int> arr;
    arr.insert(10);
    arr.insert(20);

    Array<int> arrMove(move(arr));  // ���������� ������

    ASSERT_EQ(arrMove.size(), 2);

    ASSERT_EQ(arrMove[0], 10);
    ASSERT_EQ(arrMove[1], 20);

    ASSERT_EQ(arr.size(), 0);  // �������� ������ ������ ���� ������
}

// ���� ���������� ������������ 1
TEST(ArrayTest, CopyAssignmentOperator) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);

    Array<int> arrCopy;
    arrCopy = arr;  // �������� ����� �������� ������������

    ASSERT_EQ(arrCopy.size(), 2);

    ASSERT_EQ(arrCopy[0], 10);
    ASSERT_EQ(arrCopy[1], 20);
}

// ���� ���������� ������������ 2
TEST(ArrayTest, MoveAssignmentOperator) 
{
    Array<int> arr;

    arr.insert(10);
    arr.insert(20);

    Array<int> arrMove;
    arrMove = move(arr);  // ���������� ����� �������� ������������

    ASSERT_EQ(arrMove.size(), 2);

    ASSERT_EQ(arrMove[0], 10);
    ASSERT_EQ(arrMove[1], 20);

    ASSERT_EQ(arr.size(), 0);  // �������� ������ ������ ���� ������
}

// ���� ��� ����������
TEST(ArrayTest, Iterator) 
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

