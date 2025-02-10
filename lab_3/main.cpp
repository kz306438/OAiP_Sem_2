#include "LinkedList.h"
#include "Stack.h"
#include <iostream>
#include <algorithm>
#include "Sort.h"
#include <vector>
#include <limits>
#include <random>

// Function for finding the maximum element in the stack
int findMaxElement(container::Stack<int>& stack)
{
    container::Stack<int> temp;
    int maxElement = std::numeric_limits<int>::min();
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();

        if (current > maxElement) {
            maxElement = current;
        }

        temp.push(current);
    }

    while (!temp.empty()) {
        int current = temp.top(); temp.pop();
        stack.push(current);
    }

    return maxElement;
}

// Function to move elements between the top of the stack and the maximum element to a new stack
void transferBetweenTopAndMax(container::Stack<int>& original, container::Stack<int>& newStack)
{
    int maxElement = findMaxElement(original);

    while (!original.empty()) {
        int current = original.top();
        original.pop();

        newStack.push(current);

        if (current == maxElement) {
            break;  
        }
    }
}

//===========================| Test stack with random numbers |============================//

void fillStackByRandomNumbers(container::Stack<int>& stack)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100, 100);


    std::cout << "All random elements: " << std::endl;
    for (int i = 0; i < 10; ++i) {
        int randomNumer = dis(gen);
        stack.push(randomNumer);
        std::cout << randomNumer << ' ';
    }
    std::cout << std::endl;
}

void printFinalStack(container::Stack<int>& stack)
{
    std::cout << "Elements in new stack:" << std::endl;
    while (!stack.empty()) {
        std::cout << stack.top() << " ";
        stack.pop();
    }
    std::cout << std::endl;
}

void testStackWithRandomNumbers()
{
    container::Stack<int> originalStack;
    container::Stack<int> newStack;

    fillStackByRandomNumbers(originalStack);
    transferBetweenTopAndMax(originalStack, newStack);
    printFinalStack(newStack);
}

//===========================| List sort test |============================//


void printList(const container::LinkedList<int>& list)
{
    std::cout << "Elements in list:" << std::endl;
    for (const auto& el : list)
        std::cout << el << ' ';
    std::cout << std::endl;
}

void listSortTest()
{
    container::LinkedList<int> list;
    list.push_back(10);
    list.push_back(0);
    list.push_back(14);
    list.push_back(-3);
    list.push_back(80);
    list.push_back(5);

    algorithm::sortCollection(list);
    printList(list);
}

// Client for test
int main() {
    testStackWithRandomNumbers();
    listSortTest();
}
