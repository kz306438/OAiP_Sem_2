#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
#include <random>
#include "Deque.h"
#include "Sort.h"
#include <limits>

// Function for finding the maximum element in the Deque
int findMaxElement(container::Deque<int>& deque)
{
    container::Deque<int> temp;
    int maxElement = std::numeric_limits<int>::min();

    while (!deque.empty()) {
        int current = deque.back();
        deque.pop_back();

        if (current > maxElement) {
            maxElement = current;
        }

        temp.push_back(current);
    }

    while (!temp.empty()) {
        int current = temp.back();
        temp.pop_back();
        deque.push_back(current);
    }

    return maxElement;
}

// Function to move elements between the end of the Deque and the maximum element to a new Deque
void transferBetweenTopAndMax(container::Deque<int>& original, container::Deque<int>& newDeque)
{
    int maxElement = findMaxElement(original);

    while (!original.empty()) {
        int current = original.back();
        original.pop_back();

        newDeque.push_back(current);

        if (current == maxElement) {
            break;
        }
    }
}

// Test function for filling Deque with random numbers
void fillDequeByRandomNumbers(container::Deque<int>& deque)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100, 100);

    std::cout << "All random elements: " << std::endl;
    for (int i = 0; i < 10; ++i) {
        int randomNumber = dis(gen);
        deque.push_back(randomNumber);
        std::cout << randomNumber << ' ';
    }
    std::cout << std::endl;
}

// Print function for Deque elements
void printFinalDeque(container::Deque<int>& deque)
{
    std::cout << "Elements in new Deque:" << std::endl;
    while (!deque.empty()) {
        std::cout << deque.back() << " ";
        deque.pop_back();
    }
    std::cout << std::endl;
}

// Full test of Deque with random numbers
void testDequeWithRandomNumbers()
{
    container::Deque<int> originalDeque;
    container::Deque<int> newDeque;

    fillDequeByRandomNumbers(originalDeque);
    transferBetweenTopAndMax(originalDeque, newDeque);
    printFinalDeque(newDeque);
}

// List sort print function
void printList(const container::List<int>& list)
{
    std::cout << "Elements in list:" << std::endl;
    for (const auto& el : list)
        std::cout << el << ' ';
    std::cout << std::endl;
}

// List sort test function
void listSortTest()
{
    container::List<int> list;
    list.push_back(10);
    list.push_back(0);
    list.push_back(14);
    list.push_back(-3);
    list.push_back(80);
    list.push_back(5);

    algorithm::sortCollection(list);
    printList(list);
}

#endif // TEST_UTILS_H
