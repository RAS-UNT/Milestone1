#ifndef RESERVATION_SYSTEM_H
#define RESERVATION_SYSTEM_H
#include "Reservation.h"
#include "ReservationList.h"

class ReservationSystem {
public:
    bool createReservation(const Reservation& reservation);
    bool cancelReservation(int reservationId);
    void displayReservations() const;
    bool validateReservation(const Reservation& reservation) const;

private:
    ReservationList reservations;
};

#endif

