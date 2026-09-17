#include "Reservation.h"
#include "ReservationList.h"
#include <iostream>

int main() {
    ReservationList reservations;

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

    if (!reservations.insert(firstReservation)) {
        std::cout << "First reservation is invalid.\n";
    }

    if (!reservations.insert(secondReservation)) {
        std::cout << "Second reservation is invalid.\n";
    }

    reservations.displayAll();

    return 0;
}
