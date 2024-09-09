#include "DataMonitor.h"

#include <algorithm>
#include <iostream>
#include <iterator>

#include "Student.h"

DataMonitor::DataMonitor(int cap) : capacity(cap), size(0) {
    arr = new Student[cap];
}

DataMonitor::~DataMonitor() {
    delete[] arr;
}

void DataMonitor::addItem(const Student& item) {
    if (size > capacity) {
        std::cout << "No space in data monitor" << std::endl;
        return;
    }

    arr[size++] = item;
}

Student DataMonitor::removeItem() {
    if (size == 0) {
        std::cout << "No items in array" << std::endl;
        exit(2);
    }

    return arr[--size];
}

int DataMonitor::getSize() const {
    return size;
}

void DataMonitor::display() const {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << std::endl;
    }
}