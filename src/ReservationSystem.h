#ifndef RESERVATION_SYSTEM_H
#define RESERVATION_SYSTEM_H
#include "Reservation.h"
#include "ReservationList.h"
#include "Resource.h"
#include "WaitingList.h"
#include "stack.h"
#include <string>

enum class ReservationRequestResult {
    Created,
    Waitlisted,
    InvalidReservation,
    DuplicateReservation,
    ResourceNotFound,
    ResourceUnavailable,
    CapacityExceeded,
    AlreadyWaiting
};

class ReservationSystem {
public:
    int loadResources(const std::string& filename);
    bool addResource(int id, const std::string& name, const std::string& type, int capacity);
    void displayResources() const;
    void displayResourceAvailability() const;
    void displayAvailableResources() const;
    bool resourceExists(int resourceId) const;
    bool isResourceAvailable(int resourceId) const;
    int resourceCount() const;

    bool createReservation(const Reservation& reservation);
    ReservationRequestResult requestReservation(const Reservation& reservation);
    bool cancelReservation(int reservationId);
    bool restoreMostRecentCancellation();
    bool processNextWaitingReservation(
        int resourceId,
        int reservationId,
        const std::string& date,
        const std::string& time,
        int groupSize
    );
    void displayReservations() const;
    void displayWaitingList() const;
    void displayCancellationHistory() const;
    bool addToWaitingList(int studentId, const std::string& studentName, int resourceId);
    bool removeFromWaitingList(int studentId);
    bool validateReservation(const Reservation& reservation) const;
    bool validateResourceReservation(const Reservation& reservation) const;
    bool reservationExists(int reservationId) const;
    int waitingListSize() const;
    int cancellationHistorySize() const;

private:
    ReservationList reservations;
    ResourceManager resources;
    WaitingList waitingList;
    CancellationStack cancellationHistory;

    std::string currentTimestamp() const;
};

#endif
