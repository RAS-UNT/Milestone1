#include "ReservationSystem.h"

bool ReservationSystem::createReservation(const Reservation& reservation) {
    if (!validateReservation(reservation)) {
        return false;
    }

    if (reservationExists(reservation.id)) {
        return false;
    }

    return reservations.insert(reservation);
}

bool ReservationSystem::cancelReservation(int reservationId) {
    return reservations.remove(reservationId);
}

void ReservationSystem::displayReservations() const {
    reservations.displayAll();
}

bool ReservationSystem::validateReservation(const Reservation& reservation) const {
    return isValidReservation(reservation);
}

bool ReservationSystem::reservationExists(int reservationId) const {
    return reservations.findById(reservationId) != nullptr;
}
