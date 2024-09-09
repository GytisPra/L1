#include "Student.h"
#ifndef SORTED_RESULT_MONITOR_H
#define SORTED_RESULT_MONITOR_H
class SortedResultMonitor {
   private:
    Student* arr;
    int capacity;
    int size;

    int findInsertPosition(const Student& Student) const;

   public:
    SortedResultMonitor(int cap = 10);

    ~SortedResultMonitor();

    void addItemSorted(const Student& item);

    const std::vector<Student> getItems();

    void display() const;

    void printToTxt(const std::string& filename) const;
};

#endif