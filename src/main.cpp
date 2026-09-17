#include "Reservation.h"
#include <iostream>

int main() {
    Reservation sampleReservation{
        1,
        "Sample Customer",
        "2026-09-17",
        "6:30 PM",
        4
    };

    if (isValidReservation(sampleReservation)) {
        displayReservation(sampleReservation);
    } else {
        std::cout << "Reservation is invalid.\n";
    }
    return 0;
}

