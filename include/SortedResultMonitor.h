#include <condition_variable>
#include <mutex>

#include "Word.h"
#ifndef SORTED_RESULT_MONITOR_H
#define SORTED_RESULT_MONITOR_H
class SortedResultMonitor {
   private:
    Word *arr;
    int capacity;
    int size;
    mutable std::mutex mutx;                         // mutex to protect threads from trying to reach data at the same time
    int findInsertPosition(const Word &word) const;  // Find where to insert an item in order to
                                                     // preserve the sorted order
   public:
    SortedResultMonitor(int cap = 10);     // Constructor
    ~SortedResultMonitor();                // Deconstructor
    void addItemSorted(const Word &item);  // Add an item to the array while keeping it sorted
    const Word *getItems() const;          // Return a constant pointer to the array
    void display() const;                  // Display out all items in the array
    int getSize();
};

#endif