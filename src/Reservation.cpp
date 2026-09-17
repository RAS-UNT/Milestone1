#include "Reservation.h"
#include <iostream>

bool isValidReservation(const Reservation& reservation) {
    return reservation.id > 0 &&
           !reservation.customerName.empty() &&
           !reservation.date.empty() &&
           !reservation.time.empty() &&
           reservation.partySize > 0;
}

void displayReservation(const Reservation& reservation) {
    std::cout << "Reservation #" << reservation.id << '\n'
              << "Name: " << reservation.customerName << '\n'
              << "Date: " << reservation.date << '\n'
              << "Time: " << reservation.time << '\n'
              << "Party size: " << reservation.partySize << '\n';
}

