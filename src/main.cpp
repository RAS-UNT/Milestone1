#include "ReservationSystem.h"
#include <iostream>
#include <limits>
#include <string>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool readInt(const std::string& prompt, int& value) {
    std::cout << prompt;
    std::cin >> value;

    if (std::cin.fail()) {
        clearInput();
        return false;
    }

    clearInput();
    return true;
}

std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

void displayMenu() {
    std::cout << "\nCampus Resource Reservation System\n"
              << "1. Load resources from file\n"
              << "2. Display all resources\n"
              << "3. Display resource availability\n"
              << "4. Create reservation\n"
              << "5. Cancel reservation\n"
              << "6. Display active reservations\n"
              << "7. Add student to waiting list\n"
              << "8. Process next waiting student\n"
              << "9. Display waiting list\n"
              << "10. Restore most recent cancellation\n"
              << "11. Display cancellation history\n"
              << "12. Exit\n"
              << "Choose an option: ";
}

const char* requestResultMessage(ReservationRequestResult result) {
    switch (result) {
        case ReservationRequestResult::Created:
            return "Reservation created.";
        case ReservationRequestResult::Waitlisted:
            return "Resource is not available. Student added to waiting list.";
        case ReservationRequestResult::InvalidReservation:
            return "Reservation is invalid. Check IDs, name, date, time, and group size.";
        case ReservationRequestResult::DuplicateReservation:
            return "A reservation with that ID already exists.";
        case ReservationRequestResult::ResourceNotFound:
            return "Resource ID was not found.";
        case ReservationRequestResult::ResourceUnavailable:
            return "Resource is not available.";
        case ReservationRequestResult::CapacityExceeded:
            return "Requested group size exceeds the resource capacity.";
        case ReservationRequestResult::AlreadyWaiting:
            return "That student is already on the waiting list.";
    }

    return "Reservation request could not be completed.";
}

Reservation readReservation() {
    Reservation reservation{};

    if (!readInt("Reservation ID: ", reservation.id)) {
        reservation.id = 0;
    }

    if (!readInt("Student ID: ", reservation.studentId)) {
        reservation.studentId = 0;
    }

    reservation.customerName = readLine("Student name: ");

    if (!readInt("Resource ID: ", reservation.resourceId)) {
        reservation.resourceId = 0;
    }

    reservation.date = readLine("Date (YYYY-MM-DD): ");
    reservation.time = readLine("Time: ");

    if (!readInt("Group size: ", reservation.partySize)) {
        reservation.partySize = 0;
    }

    return reservation;
}

int main() {
    ReservationSystem reservationSystem;
    int loadedResources = reservationSystem.loadResources("resources.txt");

    if (loadedResources >= 0) {
        std::cout << "Loaded " << loadedResources << " resource(s) from resources.txt.\n";
    }

    int choice = 0;

    do {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            clearInput();
            std::cout << "Please enter a number from 1 to 12.\n";
            continue;
        }

        clearInput();

        if (choice == 1) {
            std::string filename = readLine("Resource file name: ");
            int loaded = reservationSystem.loadResources(filename);

            if (loaded >= 0) {
                std::cout << "Loaded " << loaded << " resource(s).\n";
            }
        } else if (choice == 2) {
            reservationSystem.displayResources();
        } else if (choice == 3) {
            reservationSystem.displayResourceAvailability();
        } else if (choice == 4) {
            reservationSystem.displayAvailableResources();
            Reservation reservation = readReservation();
            ReservationRequestResult result = reservationSystem.requestReservation(reservation);
            std::cout << requestResultMessage(result) << '\n';
        } else if (choice == 5) {
            int reservationId = 0;

            if (!readInt("Reservation ID to cancel: ", reservationId)) {
                std::cout << "Please enter a valid reservation ID.\n";
                continue;
            }

            if (reservationSystem.cancelReservation(reservationId)) {
                std::cout << "Reservation canceled and stored in cancellation history.\n";
            } else {
                std::cout << "Reservation was not found.\n";
            }
        } else if (choice == 6) {
            reservationSystem.displayReservations();
        } else if (choice == 7) {
            int studentId = 0;
            int resourceId = 0;

            if (!readInt("Student ID: ", studentId)) {
                std::cout << "Please enter a valid student ID.\n";
                continue;
            }

            std::string studentName = readLine("Student name: ");

            if (!readInt("Resource ID: ", resourceId)) {
                std::cout << "Please enter a valid resource ID.\n";
                continue;
            }

            if (reservationSystem.addToWaitingList(studentId, studentName, resourceId)) {
                std::cout << "Student added to waiting list.\n";
            } else {
                std::cout << "Could not add student. Check resource ID and duplicate entries.\n";
            }
        } else if (choice == 8) {
            int resourceId = 0;
            int reservationId = 0;
            int groupSize = 0;

            if (!readInt("Available resource ID to assign: ", resourceId) ||
                !readInt("New reservation ID: ", reservationId)) {
                std::cout << "Please enter valid numeric IDs.\n";
                continue;
            }

            std::string date = readLine("Date (YYYY-MM-DD): ");
            std::string time = readLine("Time: ");

            if (!readInt("Group size: ", groupSize)) {
                std::cout << "Please enter a valid group size.\n";
                continue;
            }

            if (reservationSystem.processNextWaitingReservation(
                    resourceId,
                    reservationId,
                    date,
                    time,
                    groupSize
                )) {
                std::cout << "Next waiting student was assigned to the resource.\n";
            } else {
                std::cout << "Could not process waiting list for that resource.\n";
            }
        } else if (choice == 9) {
            reservationSystem.displayWaitingList();
        } else if (choice == 10) {
            if (reservationSystem.restoreMostRecentCancellation()) {
                std::cout << "Most recent cancellation restored.\n";
            } else {
                std::cout << "No cancellation could be restored.\n";
            }
        } else if (choice == 11) {
            reservationSystem.displayCancellationHistory();
        } else if (choice != 12) {
            std::cout << "Please choose a valid option.\n";
        }
    } while (choice != 12);

    std::cout << "Goodbye.\n";
    return 0;
}
