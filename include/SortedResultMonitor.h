#include <condition_variable>
#include <mutex>

#include "Student.h"
#ifndef SORTED_RESULT_MONITOR_H
#define SORTED_RESULT_MONITOR_H
class SortedResultMonitor {
   private:
    Student* arr;
    int capacity;
    int size;
    mutable std::mutex mutx;  // mutex to protect threads from trying to reach data at the same time

    int findInsertPosition(const Student& Student) const;  // Find where to insert an item in order to preserve the sorted order
   public:
    SortedResultMonitor(int cap = 10);                   // Constructor
    ~SortedResultMonitor();                              // Deconstructor
    void addItemSorted(const Student& item);             // Add an item to the array while keeping it sorted
    const std::vector<Student> getItems();               // Get all of the items from the array
    void display() const;                                // Display out all items in the array
    void printToTxt(const std::string& filename) const;  // Print all items to the specified filename
};

#endif