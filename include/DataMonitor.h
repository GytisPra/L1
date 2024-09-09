#include <iostream>
#include <vector>

#include "Student.h"

#ifndef DATA_MONITOR_H
#define DATA_MONITOR_H

class DataMonitor {
   private:
    Student* arr;
    int capacity;
    int size;

   public:
    DataMonitor(int cap = 10);

    ~DataMonitor();

    void addItem(const Student& item);

    Student removeItem();

    int getSize() const;

    void display() const;
};

#endif
