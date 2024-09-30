#include <Word.h>

#include <atomic>
#include <cmath>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <thread>

#include "DataMonitor.h"
#include "Logger.h"
#include "SortedResultMonitor.h"

#define NUM_OF_DATA_FILES 3

using json = nlohmann::json;

template <typename T>
void printToTxt(const std::string &fileword, const T &arr, int size) {
    std::ofstream outFile(fileword);
    if (!outFile) {
        std::cerr << "Error opening file: " << fileword << std::endl;
        return;
    }

    if (size == 0) {
        outFile << "No data in sortedDataMonitor";
        return;
    }

    outFile << std::setw(5) << std::left << "Nr." << "|"
            << std::setw(21) << std::left << "Žodis" << "|"
            << std::setw(10) << std::right << "Ilgis" << "|"
            << std::setw(23) << std::right << "Žodžio vertė" << "|"
            << " " << "Palindromas" << std::endl;
    outFile << std::string(80, '-') << std::endl;

    for (int i = 0; i < size; i++) {
        outFile << std::setw(5) << std::left << std::to_string(i + 1) + ". " << "|"
                << std::setw(20) << std::left << arr[i].word << "|"
                << std::setw(10) << std::right << arr[i].length << "|"
                << std::setw(20) << std::right << arr[i].wordValue << "|"
                << " " << arr[i].isPalindrome << std::endl;
    }

    outFile.close();
}

void isPalindrome(Word &word) {
    int length = word.length;

    for (int i = 0; i < 10000000; i++) {
        for (int i = 0; i < length; i++) {
            if (word.word.at(i) != word.word.at(length - i - 1)) {
                word.isPalindrome = "Ne";
                // return;
            }
        }
    }
    if (word.isPalindrome == "Nežinome") {
        word.isPalindrome = "Taip";
    }
}

// Worker thread function
void consumer(DataMonitor &monitor, SortedResultMonitor &sortedMonitor) {
    while (true) {
        Word word = monitor.removeItem();

        // A default Word is returned when the monitor has no data and the main
        // thread has finished
        if (word.isDefault()) {
            logMsg("Thread ", std::this_thread::get_id(), " has finished");
            break;
        }

        isPalindrome(word);

        if (word.isPalindrome == "Taip" && word.wordValue >= 100) {
            sortedMonitor.addItemSorted(word);
        }
    }
}

int main() {
    for (int i = 1; i <= 3; i++) {
        std::string dataFileName = "IFF-2-8_PranauskasG_L1_dat_" + std::to_string(i) + ".json";
        std::cout << std::string(30, '-') << std::endl;
        std::cout << "Data file name: " << dataFileName << std::endl;
        std::cout << std::string(30, '-') << std::endl;

        DataMonitor monitor(12);
        SortedResultMonitor sortedMonitor(30);
        int NUM_OF_THREADS = 0;

        std::ifstream file("../" + dataFileName);
        if (!file.is_open()) {
            std::cerr << "Could not open the file!" << std::endl;
            return 1;
        }

        nlohmann::json j;
        file >> j;

        std::vector<Word> words = j["words"];

        printToTxt("IFF-2-8_PranauskasG_L1a_prad-" + std::to_string(i) + ".txt", words, words.size());

        // Calculate based on data how many worker threads to create
        NUM_OF_THREADS = std::max(2, static_cast<int>(std::ceil(words.size() / 4.0)));

        logMsg("Creating ", NUM_OF_THREADS, " worker threads");
        std::vector<std::thread> consumers;
        for (int i = 0; i < NUM_OF_THREADS; i++) {
            // Start worker threads
            consumers.emplace_back(consumer, std::ref(monitor), std::ref(sortedMonitor));
        }

        // Start adding items to the data monitor
        for (const auto &word : words) {
            monitor.addItem(word);
        }

        // Signal that the main thread has finished its job
        monitor.setDone();

        // Wait for all consumer threads to finish
        for (auto &t : consumers) {
            t.join();
        }

        logMsg("Writing results to 'IFF-2-8_PranauskasG_L1a_rez-" + std::to_string(i) + ".txt'");
        printToTxt("IFF-2-8_PranauskasG_L1a_rez-" + std::to_string(i) + ".txt",
                   sortedMonitor.getItems(), sortedMonitor.getSize());

        consumers.clear();
    }

    system("pause");

    return 0;
}