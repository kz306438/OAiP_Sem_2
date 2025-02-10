#ifndef FORWARD_MERGE_SORT_H
#define FORWARD_MERGE_SORT_H

#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <list>

namespace algorithm
{
    // Merges two sorted halves of a container
    template <typename ForwardIterator>
    ForwardIterator merge(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
        std::vector<typename std::iterator_traits<ForwardIterator>::value_type> temp; // Temp vector for merged elements
        ForwardIterator left = first, right = middle;

        // Merge elements from both halves into the temp vector
        while (left != middle && right != last) {
            if (*left < *right) {
                temp.push_back(*left);
                ++left;
            }
            else {
                temp.push_back(*right);
                ++right;
            }
        }

        // Add remaining elements from the left half
        while (left != middle) {
            temp.push_back(*left);
            ++left;
        }

        // Add remaining elements from the right half
        while (right != last) {
            temp.push_back(*right);
            ++right;
        }

        // Copy merged elements back to the original container
        for (auto it = temp.begin(); it != temp.end(); ++it) {
            *first++ = *it;
        }

        return first;
    }

    // Recursively divides and sorts the container using merge sort
    template <typename ForwardIterator>
    void mergeSort(ForwardIterator first, ForwardIterator last) {
        if (std::distance(first, last) <= 1) return; 

        // Find the middle point of the container
        ForwardIterator middle = first;
        std::advance(middle, std::distance(first, last) / 2);

        // Recursively sort both halves
        mergeSort(first, middle);
        mergeSort(middle, last);

        // Merge the two sorted halves
        merge(first, middle, last);
    }

    // Sorts a container using merge sort
    template <typename Container>
    void sortCollection(Container& container) {
        mergeSort(std::begin(container), std::end(container));
    }

} // algorithm

#endif // FORWARD_MERGE_SORT_H
