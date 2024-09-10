#include "DataMonitor.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>

#include "Logger.h"
#include "Student.h"

DataMonitor::DataMonitor(int cap) : capacity(cap), size(0) {
    arr = new Student[cap];
}

DataMonitor::~DataMonitor() {
    delete[] arr;
}

void DataMonitor::addItem(const Student& student) {
    std::unique_lock<std::mutex> lock(mutx);

    cv_arr_not_full.wait(lock, [this]() {
        return size < capacity;
    });

    arr[size++] = student;

    logMsg("Added student to data monitor, current size: " + std::to_string(size));

    cv_arr_not_empty.notify_one();
}

Student DataMonitor::removeItem() {
    std::unique_lock<std::mutex> lock(mutx);

    cv_arr_not_empty.wait(lock, [this]() {
        return size > 0;
    });

    logMsg("Removed student from data monitor, current size: " + std::to_string(size - 1));

    cv_arr_not_full.notify_one();

    return arr[--size];
}

int DataMonitor::getSize() const {
    std::unique_lock<std::mutex> lock(mutx);

    return size;
}

void DataMonitor::display() const {
    std::unique_lock<std::mutex> lock(mutx);

    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << std::endl;
    }
}