#include "SortedResultMonitor.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <thread>

#include "Logger.h"
#include "Word.h"

SortedResultMonitor::SortedResultMonitor(int cap) : capacity(cap), size(0) {
    arr = new Word[cap];
}

SortedResultMonitor::~SortedResultMonitor() { delete[] arr; }

void SortedResultMonitor::addItemSorted(const Word& word) {
    if (size > capacity) {
        std::cout << "No space in sorted result monitor" << std::endl;
        return;
    }

    int pos = -1;

    {
        std::unique_lock<std::mutex> lock(mutx);

        pos = SortedResultMonitor::findInsertPosition(word);

        for (int i = size; i > pos; i--) {
            arr[i] = arr[i - 1];
        }

        arr[pos] = word;
        ++size;
    }

    logMsg("Thread ", std::this_thread::get_id(),
           " Added word to result data monitor, current size ",
           std::to_string(size));
}

int SortedResultMonitor::findInsertPosition(const Word& word) const {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] < word) {
            left = mid + 1;
        } else if (word < arr[mid]) {
            right = mid - 1;
        } else {
            return mid;
        }
    }

    return left;
}

const Word* SortedResultMonitor::getItems() const { return arr; }
int SortedResultMonitor::getSize() { return size; }

void SortedResultMonitor::display() const {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << std::endl;
    }
}