#include "Reservation.h"
#include "ReservationSystem.h"
#include <iostream>

int main() {
    ReservationSystem reservationSystem;
    Reservation firstReservation{
        1,
        "Sample Customer",
        "2026-09-17",
        "6:30 PM",
        4
    };

    Reservation secondReservation{
        2,
        "Another Customer",
        "2026-09-18",
        "7:00 PM",
        2
    };

    Reservation duplicateReservation{
        2,
        "Duplicate Customer",
        "2026-09-19",
        "8:00 PM",
        3
    };

    if (!reservationSystem.createReservation(firstReservation)) {
        std::cout << "First reservation is invalid.\n";
    }

    if (!reservationSystem.createReservation(secondReservation)) {
        std::cout << "Second reservation is invalid.\n";
    }

    if (!reservationSystem.createReservation(duplicateReservation)) {
        std::cout << "Duplicate reservation was not added.\n";
    }

    std::cout << "Current reservations:\n";
    reservationSystem.displayReservations();

    if (reservationSystem.cancelReservation(1)) {
        std::cout << "Reservation #1 removed.\n\n";
    } else {
        std::cout << "Reservation #1 was not found.\n\n";
    }

    std::cout << "Reservations after removal:\n";
    reservationSystem.displayReservations();

    return 0;
}
