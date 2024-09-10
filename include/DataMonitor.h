#include <condition_variable>
#include <iostream>
#include <mutex>
#include <vector>

#include "Student.h"

#ifndef DATA_MONITOR_H
#define DATA_MONITOR_H

class DataMonitor {
   private:
    Student* arr;
    int capacity;
    int size;
    mutable std::mutex mutx;
    std::condition_variable cv_arr_not_full;   // Conditional variable to signal that the array is not full
    std::condition_variable cv_arr_not_empty;  // Conditional variable to signal that the array is not empty
    bool done = false;                         // Flag used to signal to all threads that the main thread has finished adding all items to array

   public:
    DataMonitor(int cap = 10);

    ~DataMonitor();

    void addItem(const Student& item);

    Student removeItem();

    int getSize() const;

    void display() const;

    void setDone();
};

#endif
