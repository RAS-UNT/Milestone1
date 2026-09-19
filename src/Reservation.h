#ifndef RESERVATION_H
#define RESERVATION_H
#include <string>

struct Reservation {
    int id;
    std::string customerName;
    std::string date;
    std::string time;
    int partySize;
};

bool isValidReservation(const Reservation& reservation);
void displayReservation(const Reservation& reservation);

#endif

