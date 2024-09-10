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

/*
 *   TODO:
 *   Sugalvoti savo duomenų strukūrą
 *   Sugalvoti funkcija kur truputi užtruktu
 *   Pagal šia funkciją filtruoti savo duomenis
 */

// Worker thread function
void consumer(DataMonitor& monitor, SortedResultMonitor& sortedMonitor) {
    while (true) {
        Student student = monitor.removeItem();

        // A default student is returned when the monitor has no data and the main thread has finished
        if (student.isDefault()) {
            logMsg("Thread ", std::this_thread::get_id(), " has finished");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Simulate work

        // Random filtering check
        if (student.Grade > 8) {
            sortedMonitor.addItemSorted(student);
        }
    }
}

int main() {
    int NUM_OF_THREADS = 0;

    DataMonitor monitor(15);
    SortedResultMonitor sortedMonitor(30);

    std::ifstream file("../example.json");
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    nlohmann::json j;
    file >> j;

    std::vector<Student> students = j["students"];

    // Calculate based on data how many worker threads to create
    NUM_OF_THREADS = std::max(2, static_cast<int>(std::ceil(students.size() / 4.0)));

    logMsg("Creating ", NUM_OF_THREADS, " worker threads");

    std::vector<std::thread> consumers;
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        // Start worker threads
        consumers.emplace_back(consumer, std::ref(monitor), std::ref(sortedMonitor));
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

    logMsg("Writing results to 'Rezultatai.txt'");
    sortedMonitor.printToTxt("Rezultatai.txt");

    system("pause");

    return 0;
}