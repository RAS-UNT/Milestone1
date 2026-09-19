#include "ReservationSystem.h"
#include <ctime>
#include <iomanip>
#include <sstream>

int ReservationSystem::loadResources(const std::string& filename) {
    return resources.loadFromFile(filename);
}

bool ReservationSystem::addResource(
    int id,
    const std::string& name,
    const std::string& type,
    int capacity
) {
    return resources.addResource(id, name, type, capacity);
}

void ReservationSystem::displayResources() const {
    resources.displayAll();
}

void ReservationSystem::displayResourceAvailability() const {
    resources.displayAvailability();
}

void ReservationSystem::displayAvailableResources() const {
    resources.displayAvailableOnly();
}

bool ReservationSystem::resourceExists(int resourceId) const {
    return resources.exists(resourceId);
}

bool ReservationSystem::isResourceAvailable(int resourceId) const {
    return resources.isAvailable(resourceId);
}

int ReservationSystem::resourceCount() const {
    return resources.count();
}

bool ReservationSystem::createReservation(const Reservation& reservation) {
    if (!validateReservation(reservation)) {
        return false;
    }

    if (reservationExists(reservation.id)) {
        return false;
    }

    if (reservation.resourceId > 0) {
        const Resource* resource = resources.findById(reservation.resourceId);

        if (resource == nullptr || !resource->available ||
            reservation.partySize > resource->capacity) {
            return false;
        }
    }

    if (!reservations.insert(reservation)) {
        return false;
    }

    if (reservation.resourceId > 0) {
        resources.markReserved(reservation.resourceId);
    }

    return true;
}

ReservationRequestResult ReservationSystem::requestReservation(const Reservation& reservation) {
    if (!validateResourceReservation(reservation)) {
        return ReservationRequestResult::InvalidReservation;
    }

    if (reservationExists(reservation.id)) {
        return ReservationRequestResult::DuplicateReservation;
    }

    const Resource* resource = resources.findById(reservation.resourceId);

    if (resource == nullptr) {
        return ReservationRequestResult::ResourceNotFound;
    }

    if (reservation.partySize > resource->capacity) {
        return ReservationRequestResult::CapacityExceeded;
    }

    if (!resource->available) {
        if (waitingList.enqueue(
                reservation.studentId,
                reservation.customerName,
                reservation.resourceId
            )) {
            return ReservationRequestResult::Waitlisted;
        }

        return ReservationRequestResult::AlreadyWaiting;
    }

    return createReservation(reservation)
        ? ReservationRequestResult::Created
        : ReservationRequestResult::InvalidReservation;
}

bool ReservationSystem::cancelReservation(int reservationId) {
    Reservation removedReservation{};

    if (!reservations.remove(reservationId, removedReservation)) {
        return false;
    }

    cancellationHistory.push(removedReservation, currentTimestamp());

    if (removedReservation.resourceId > 0) {
        resources.markAvailable(removedReservation.resourceId);
    }

    return true;
}

bool ReservationSystem::restoreMostRecentCancellation() {
    Reservation reservation{};

    if (!cancellationHistory.peek(reservation)) {
        return false;
    }

    if (!validateReservation(reservation) || reservationExists(reservation.id)) {
        return false;
    }

    if (reservation.resourceId > 0) {
        const Resource* resource = resources.findById(reservation.resourceId);

        if (resource == nullptr || !resource->available ||
            reservation.partySize > resource->capacity) {
            return false;
        }
    }

    cancellationHistory.restore(reservation);

    if (!reservations.insert(reservation)) {
        return false;
    }

    if (reservation.resourceId > 0) {
        resources.markReserved(reservation.resourceId);
    }

    return true;
}

bool ReservationSystem::processNextWaitingReservation(
    int resourceId,
    int reservationId,
    const std::string& date,
    const std::string& time,
    int groupSize
) {
    if (reservationExists(reservationId)) {
        return false;
    }

    const Resource* resource = resources.findById(resourceId);

    if (resource == nullptr || !resource->available || groupSize > resource->capacity) {
        return false;
    }

    WaitingEntry entry{};

    if (!waitingList.dequeueForResource(resourceId, entry)) {
        return false;
    }

    Reservation reservation{
        reservationId,
        entry.studentName,
        date,
        time,
        groupSize,
        resourceId,
        entry.studentId
    };

    if (requestReservation(reservation) != ReservationRequestResult::Created) {
        waitingList.enqueue(entry.studentId, entry.studentName, entry.resourceId);
        return false;
    }

    return true;
}

void ReservationSystem::displayReservations() const {
    reservations.displayAll();
}

void ReservationSystem::displayWaitingList() const {
    waitingList.display();
}

void ReservationSystem::displayCancellationHistory() const {
    cancellationHistory.display();
}

bool ReservationSystem::addToWaitingList(
    int studentId,
    const std::string& studentName,
    int resourceId
) {
    return resourceExists(resourceId) && waitingList.enqueue(studentId, studentName, resourceId);
}

bool ReservationSystem::removeFromWaitingList(int studentId) {
    return waitingList.removeStudent(studentId);
}

bool ReservationSystem::validateReservation(const Reservation& reservation) const {
    return isValidReservation(reservation);
}

bool ReservationSystem::validateResourceReservation(const Reservation& reservation) const {
    return validateReservation(reservation) &&
           reservation.resourceId > 0 &&
           reservation.studentId > 0;
}

bool ReservationSystem::reservationExists(int reservationId) const {
    return reservations.findById(reservationId) != nullptr;
}

int ReservationSystem::waitingListSize() const {
    return waitingList.size();
}

int ReservationSystem::cancellationHistorySize() const {
    return cancellationHistory.size();
}

std::string ReservationSystem::currentTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    if (localTime == nullptr) {
        return "unknown";
    }

    std::ostringstream output;
    output << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return output.str();
}
