#include <Logger.h>
#include <Student.h>

#include <atomic>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <thread>

#include "DataMonitor.h"
#include "SortedResultMonitor.h"

using json = nlohmann::json;

void filter(DataMonitor& monitor, SortedResultMonitor& sortedMonitor);

void filter(DataMonitor& monitor, SortedResultMonitor& sortedMonitor) {
    Student stud = monitor.removeItem();

    if (stud.Grade > 8) {
        sortedMonitor.addItemSorted(stud);
    }
}

void consumer(DataMonitor& monitor) {
    while (true) {
        Student student = monitor.removeItem();

        if (student.isDefault()) {
            logMsg("Thread ", std::this_thread::get_id(), " has finished");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    DataMonitor monitor(15);
    // SortedResultMonitor sortedMonitor(30);

    std::ifstream file("../example.json");
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    nlohmann::json j;
    file >> j;

    std::vector<Student> students = j["students"];

    std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

    std::thread cons1(consumer, std::ref(monitor));
    std::thread cons2(consumer, std::ref(monitor));

    for (const auto& student : students) {
        monitor.addItem(student);
    }

    monitor.setDone();

    cons1.join();
    cons2.join();

    return 0;
}