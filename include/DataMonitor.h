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
    DataMonitor(int cap = 10);          // Constructor
    ~DataMonitor();                     // Deconstructor
    void addItem(const Student& item);  // Function to add items into the array
    Student removeItem();               // Function to remove and return the latest item from the array
    int getSize() const;                // Returns how many items are in an array
    void display() const;               // Used to print out what is in the data monitor
    void setDone();                     // Function to signal threads that the main thread has finished adding items to the data monitor
};

#endif
