#include <Student.h>

#include <atomic>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <thread>

#include "DataMonitor.h"
#include "SortedResultMonitor.h"

using json = nlohmann::json;

std::atomic<bool> done(false);

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

void consumer(DataMonitor& monitor) {
    while (true) {
        if (done && monitor.getSize() == 0) {
            break;
        }

        monitor.removeItem();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

    std::thread cons(consumer, std::ref(monitor));

    for (const auto& student : students) {
        monitor.addItem(student);
    }

    done = true;

    cons.join();

    return 0;
}