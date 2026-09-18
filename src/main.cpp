#include "Reservation.h"
#include "ReservationSystem.h"
#include <iostream>
#include <limits>
#include <string>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMenu() {
    std::cout << "\nReservation Menu\n"
              << "1. Create reservation\n"
              << "2. Cancel reservation\n"
              << "3. Display reservations\n"
              << "4. Exit\n"
              << "Choose an option: ";
}

Reservation readReservation() {
    Reservation reservation{};

    std::cout << "Reservation ID: ";
    std::cin >> reservation.id;
    clearInput();

    std::cout << "Customer name: ";
    std::getline(std::cin, reservation.customerName);

    std::cout << "Date: ";
    std::getline(std::cin, reservation.date);

    std::cout << "Time: ";
    std::getline(std::cin, reservation.time);

    std::cout << "Party size: ";
    std::cin >> reservation.partySize;
    clearInput();

    return reservation;
}

int main() {
    ReservationSystem reservationSystem;
    int choice = 0;

    do {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            clearInput();
            std::cout << "Please enter a number from 1 to 4.\n";
            continue;
        }

        clearInput();

        if (choice == 1) {
            Reservation reservation = readReservation();

            if (reservationSystem.reservationExists(reservation.id)) {
                std::cout << "A reservation with that ID already exists.\n";
            } else if (reservationSystem.createReservation(reservation)) {
                std::cout << "Reservation created.\n";
            } else {
                std::cout << "Reservation is invalid.\n";
            }
        } else if (choice == 2) {
            int reservationId = 0;

            std::cout << "Reservation ID to cancel: ";
            std::cin >> reservationId;
            if (std::cin.fail()) {
                clearInput();
                std::cout << "Please enter a valid reservation ID.\n";
                continue;
            }

            clearInput();

            if (reservationSystem.cancelReservation(reservationId)) {
                std::cout << "Reservation canceled.\n";
            } else {
                std::cout << "Reservation was not found.\n";
            }
        } else if (choice == 3) {
            reservationSystem.displayReservations();
        } else if (choice != 4) {
            std::cout << "Please choose a valid option.\n";
        }
    } while (choice != 4);

    std::cout << "Goodbye.\n";
    return 0;
}
