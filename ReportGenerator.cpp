// ReportGenerator.cpp
// Implements the four system reports. All reports are read-only: they
// only read data from the ReservationSystem and never change it.
// Author: Sulav

#include "ReportGenerator.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

// Report 1: all active reservations and the total count.
// Time complexity: O(n) for n active reservations.
void ReportGenerator::activeReservations(const ReservationSystem& sys) {
    std::cout << "\n=== Active Reservations Report ===\n";
    const ReservationList& list = sys.getReservations();

    if (list.isEmpty()) {
        std::cout << "No active reservations.\n";
        return;
    }

    int count = 0;
    list.traverse([&count](const Reservation& r) {
        displayReservation(r);
        std::cout << '\n';
        count++;
    });
    std::cout << "Total active reservations: " << count << "\n";
}

// Report 2: each resource and how many reservations it has had.
// Uses a sorted copy (by ID) so the original resource list is not changed.
// Time complexity: O(r log r) for r resources.
void ReportGenerator::resourceUtilization(const ReservationSystem& sys) {
    const ResourceManager& rm = sys.getResources();
    std::cout << "\n=== Resource Utilization Report ===\n";

    if (rm.count() == 0) {
        std::cout << "No resources loaded.\n";
        return;
    }

    std::vector<Resource> list = rm.sortedCopy(SORT_BY_ID, true);
    int total = 0;

    std::cout << std::left << std::setw(6) << "ID" << std::setw(24) << "Name"
              << std::setw(12) << "Status" << "Reservations\n";
    std::cout << std::string(52, '-') << "\n";

    for (size_t i = 0; i < list.size(); i++) {
        std::cout << std::left << std::setw(6) << list[i].id
                  << std::setw(24) << list[i].name
                  << std::setw(12) << (list[i].available ? "Available" : "Reserved")
                  << list[i].timesReserved << "\n";
        total += list[i].timesReserved;
    }
    std::cout << std::string(52, '-') << "\n";
    std::cout << "Total reservations made: " << total << "\n";
}

// Report 3: resources ranked from most to least reserved (uses Merge Sort).
// Time complexity: O(r log r) for r resources.
void ReportGenerator::mostRequested(const ReservationSystem& sys, int topN) {
    const ResourceManager& rm = sys.getResources();
    std::cout << "\n=== Most Requested Resources ===\n";

    if (rm.count() == 0) {
        std::cout << "No resources loaded.\n";
        return;
    }

    std::vector<Resource> list = rm.sortedCopy(SORT_BY_POPULARITY, false);

    if (list[0].timesReserved == 0) {
        std::cout << "No reservations have been made yet.\n";
        return;
    }

    int shown = (topN < (int)list.size()) ? topN : (int)list.size();
    for (int i = 0; i < shown; i++) {
        std::cout << (i + 1) << ". [" << list[i].id << "] " << list[i].name
                  << " - " << list[i].timesReserved << " reservation(s)\n";
    }
}

// Report 4: how many students are waiting for each resource.
// Time complexity: O(r log r + r * w) for r resources and w waiting students.
void ReportGenerator::waitingListStatistics(const ReservationSystem& sys) {
    const WaitingList& wl = sys.getWaitingList();
    const ResourceManager& rm = sys.getResources();
    std::cout << "\n=== Waiting-List Statistics ===\n";

    std::cout << "Total students waiting: " << wl.size() << "\n";
    if (wl.isEmpty()) {
        std::cout << "No students are currently waiting.\n";
        return;
    }

    std::vector<Resource> list = rm.sortedCopy(SORT_BY_ID, true);
    int longestId = -1, longestCount = 0;

    std::cout << std::left << std::setw(6) << "ID" << std::setw(24) << "Name"
              << "Students Waiting\n";
    std::cout << std::string(46, '-') << "\n";

    for (size_t i = 0; i < list.size(); i++) {
        int n = wl.countForResource(list[i].id);
        std::cout << std::left << std::setw(6) << list[i].id
                  << std::setw(24) << list[i].name << n << "\n";
        if (n > longestCount) { longestCount = n; longestId = list[i].id; }
    }
    if (longestId != -1) {
        std::cout << "Longest waiting line: resource " << longestId
                  << " (" << longestCount << " student(s))\n";
    }
}

// Reports submenu: validates input and loops until the user chooses 0.
void ReportGenerator::runReportMenu(const ReservationSystem& sys) {
    int choice = -1;
    do {
        std::cout << "\n===== Reports =====\n"
                  << "1. Active reservations\n"
                  << "2. Resource utilization\n"
                  << "3. Most requested resources\n"
                  << "4. Waiting-list statistics\n"
                  << "0. Back\n"
                  << "Enter Choice: ";

        std::string line;
        if (!std::getline(std::cin, line)) return;
        std::istringstream input(line);
        if (!(input >> choice) || !(input >> std::ws).eof()) {
            std::cout << "Invalid input. Please enter a number.\n";
            choice = -1;
            continue;
        }

        if (choice == 1) activeReservations(sys);
        else if (choice == 2) resourceUtilization(sys);
        else if (choice == 3) mostRequested(sys);
        else if (choice == 4) waitingListStatistics(sys);
        else if (choice != 0) std::cout << "Invalid choice. Enter a number from 0 to 4.\n";
    } while (choice != 0);
}
