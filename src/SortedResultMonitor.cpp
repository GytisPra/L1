#include "SortedResultMonitor.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <thread>

#include "Logger.h"
#include "Student.h"

SortedResultMonitor::SortedResultMonitor(int cap) : capacity(cap), size(0) {
    arr = new Student[cap];
}

SortedResultMonitor::~SortedResultMonitor() {
    delete[] arr;
}

void SortedResultMonitor::addItemSorted(const Student& student) {
    if (size > capacity) {
        std::cout << "No space in sorted result monitor" << std::endl;
        return;
    }

    int pos = SortedResultMonitor::findInsertPosition(student);

    {
        std::unique_lock<std::mutex> lock(mutx);
        for (int i = size; i > pos; i--) {
            arr[i] = arr[i - 1];
        }

        arr[pos] = student;
        ++size;
    }

    logMsg("Thread ", std::this_thread::get_id(), " Added student to data monitor, current size ", std::to_string(size));
}

int SortedResultMonitor::findInsertPosition(const Student& student) const {
    std::unique_lock<std::mutex> lock(mutx);

    int left = 0, right = size;

    while (left <= right) {
        int mid = ceil((left + (right - left)) / 2);

        if (left == right) {
            return left;
        }
        if (mid + 1 == left) {
            return right;
        }

        if (arr[mid] < student) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return left;
}

const std::vector<Student> SortedResultMonitor::getItems() {
    return std::vector<Student>(arr, arr + size);
}

void SortedResultMonitor::display() const {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << std::endl;
    }
}

void SortedResultMonitor::printToTxt(const std::string& filename) const {
    if (size == 0) {
        std::cout << "No items in sorted result monitor" << std::endl;
        return;
    }

    std::ofstream resultsFile;
    resultsFile.open(filename);

    if (!resultsFile.is_open()) {
        std::cerr << "Could not open the file to write results" << std::endl;
        return;
    }

    resultsFile << std::setw(10) << "Name|" << std::setw(10) << "Year|" << std::setw(10) << "Grade" << std::endl;
    resultsFile << "----------------------------------------------------------------------------------------------" << std::endl;

    for (int i = 0; i < size; i++) {
        resultsFile << std::setw(10) << arr[i].Name << "|"
                    << std::setw(10) << arr[i].Year << "|"
                    << std::setw(10) << arr[i].Grade << std::endl;
    }

    resultsFile.close();
}
