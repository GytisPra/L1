#include <Student.h>

#include <fstream>
#include <iostream>
#include <json.hpp>
#include <thread>

#include "DataMonitor.h"
#include "SortedResultMonitor.h"
using json = nlohmann::json;

void filter(DataMonitor& monitor, SortedResultMonitor& sortedMonitor);

void filter(DataMonitor& monitor, SortedResultMonitor& sortedMonitor) {
    if (monitor.getSize() <= 0) {
        std::cout << "No items left in data monitor" << std::endl;
        return;
    }

    Student stud = monitor.removeItem();

    if (stud.Grade > 8) {
        sortedMonitor.addItemSorted(stud);
    }
}

int main() {
    DataMonitor monitor(10);
    SortedResultMonitor sortedMonitor(30);

    std::ifstream file("../example.json");
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    nlohmann::json j;
    file >> j;

    for (const auto& studentJson : j["students"]) {
        Student student = studentJson.get<Student>();
        monitor.addItem(student);

        filter(monitor, sortedMonitor);
    }

    sortedMonitor.display();
    sortedMonitor.printToTxt("rezultatai.txt");

    return 0;
}