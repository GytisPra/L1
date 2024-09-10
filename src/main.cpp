#include <Student.h>

#include <atomic>
#include <cmath>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <thread>

#include "DataMonitor.h"
#include "Logger.h"
#include "SortedResultMonitor.h"

using json = nlohmann::json;

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
    int NUM_OF_THREADS = 0;

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

    // Calculate based on data how many worker threads to create
    if (students.size() <= 2 && students.size() <= 0) {
        NUM_OF_THREADS = 2;
    } else {
        NUM_OF_THREADS = ceil(students.size() / 4);
    }

    // Start worker threads
    logMsg("Creating ", NUM_OF_THREADS, " worker threads");

    std::vector<std::thread> consumers;
    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        consumers.emplace_back(consumer, std::ref(monitor));
    }

    // Start adding items to the data monitor
    for (const auto& student : students) {
        monitor.addItem(student);
    }

    // Signal that the main thread has finished its job
    monitor.setDone();

    // Wait for all consumer threads to finish
    for (auto& t : consumers) {
        t.join();
    }

    // Deconstruct the data monitor
    monitor.~DataMonitor();

    return 0;
}