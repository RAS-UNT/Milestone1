#include "SortMenu.h"
#include <iostream>
#include <limits>

static bool readChoice(const char* prompt, int lo, int hi, int& out) {
    while (true) {
        std::cout << prompt;
        if (std::cin >> out) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (out >= lo && out <= hi) return true;
            std::cout << "Invalid choice. Enter a number from " << lo << " to " << hi << ".\n";
        } else {
            if (std::cin.eof()) return false;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
        }
    }
}

void runSortMenu(ResourceManager& rm) {
    if (rm.count() == 0) {
        std::cout << "No resources loaded to sort.\n";
        return;
    }

    std::cout << "\n===== Sort Resources =====\n"
              << "1. By ID\n"
              << "2. By Name\n"
              << "3. By Type\n"
              << "4. By Capacity\n"
              << "5. By Popularity (times reserved)\n"
              << "0. Back\n";

    int field;
    if (!readChoice("Enter Choice: ", 0, 5, field) || field == 0) return;

    SortKey key = SORT_BY_ID;
    switch (field) {
        case 2: key = SORT_BY_NAME;       break;
        case 3: key = SORT_BY_TYPE;       break;
        case 4: key = SORT_BY_CAPACITY;   break;
        case 5: key = SORT_BY_POPULARITY; break;
        default: key = SORT_BY_ID;        break;
    }

    std::cout << "1. Ascending\n2. Descending\n";
    int dir;
    if (!readChoice("Enter Choice: ", 1, 2, dir)) return;
    bool ascending = (dir == 1);

    rm.sortResources(key, ascending);
    std::cout << "\nResources sorted by " << ResourceManager::sortKeyName(key)
              << (ascending ? " (ascending)" : " (descending)")
              << " using Merge Sort.\n";
    rm.displayAll();
}
