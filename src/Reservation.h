#ifndef RESERVATION_H
#define RESERVATION_H
#include <string>

struct Reservation {
    int id = 0;
    std::string customerName;
    std::string date;
    std::string time;
    int partySize = 0;
    int resourceId = 0;
    int studentId = 0;
};

bool isValidReservation(const Reservation& reservation);
void displayReservation(const Reservation& reservation);

#endif
