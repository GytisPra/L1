#include "DataMonitor.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <thread>

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
        if (size >= capacity) {
            logMsg("Thread ", std::this_thread::get_id(), " waiting for space in the data monitor");
        }
        return size < capacity || done;
    });

    if (done) {
        return;
    }

    arr[size++] = student;

    logMsg("Thread ", std::this_thread::get_id(), " Added student to data monitor, current size ", std::to_string(size));

    cv_arr_not_empty.notify_one();
}

Student DataMonitor::removeItem() {
    std::unique_lock<std::mutex> lock(mutx);

    cv_arr_not_empty.wait(lock, [this]() {
        if (size == 0 && !done) {
            logMsg("Thread ", std::this_thread::get_id(), " waiting for the data monitor to not be empty");
        }

        return size > 0 || done;
    });

    if (size == 0 && done) {
        return Student{};
    }

    logMsg("Thread ", std::this_thread::get_id(), " Removed student from data monitor, current size ", std::to_string(size));

    cv_arr_not_full.notify_one();

    return arr[--size];
}

void DataMonitor::setDone() {
    std::unique_lock lock(mutx);

    logMsg("Thread ", std::this_thread::get_id(), " signaled to all threads that it has finished adding all items to data monitor");

    done = true;
    cv_arr_not_empty.notify_all();
    cv_arr_not_full.notify_all();
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