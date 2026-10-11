#include "ReservationList.h"
#include "ReservationSystem.h"

#include <iostream>
#include <string>

Reservation makeReservation(
    int id,
    const std::string& customerName,
    const std::string& date,
    const std::string& time,
    int partySize
) {
    return Reservation{id, customerName, date, time, partySize};
}

void expect(bool condition, const std::string& testName, int& failedTests) {
    if (!condition) {
        std::cout << "FAILED: " << testName << '\n';
        ++failedTests;
    }
}

void testValidationEdgeCases(int& failedTests) {
    expect(
        isValidReservation(makeReservation(1, "Rami", "2028-02-29", "6:00 PM", 2)),
        "2028-02-29 is valid",
        failedTests
    );
    expect(
        !isValidReservation(makeReservation(2, "Rami", "2027-02-29", "6:00 PM", 2)),
        "2027-02-29 is invalid",
        failedTests
    );
    expect(
        !isValidReservation(makeReservation(3, "Rami", "2025-02-30", "6:00 PM", 2)),
        "2025-02-30 is invalid",
        failedTests
    );
    expect(
        !isValidReservation(makeReservation(4, "Rami", "2026-09-18", "6:00 PM", 0)),
        "party size 0 is invalid",
        failedTests
    );
    expect(
        !isValidReservation(makeReservation(5, "Rami", "2026-09-18", "6:00 PM", -3)),
        "negative party size is invalid",
        failedTests
    );
    expect(
        !isValidReservation(makeReservation(6, "   ", "2026-09-18", "6:00 PM", 2)),
        "whitespace-only customer name is invalid",
        failedTests
    );
}

void testDuplicateIdRejectionUsesReservationSystem(int& failedTests) {
    ReservationSystem system;

    expect(
        system.createReservation(makeReservation(10, "Rami", "2026-09-18", "6:00 PM", 2)),
        "first reservation with ID 10 is created",
        failedTests
    );
    expect(
        !system.createReservation(makeReservation(10, "Duplicate", "2026-09-19", "7:00 PM", 4)),
        "duplicate ID is rejected by createReservation",
        failedTests
    );

    const Reservation* found = system.findReservationById(10);
    expect(
        found != nullptr && found->customerName == "Rami" && found->partySize == 2,
        "search returns the original reservation after duplicate ID rejection",
        failedTests
    );
}

void testReservationSearch(int& failedTests) {
    ReservationSystem system;

    expect(
        system.findReservationById(40) == nullptr,
        "search returns no match for an empty list",
        failedTests
    );

    const Reservation entries[] = {
        makeReservation(40, "Head", "2026-10-11", "10:00 AM", 2),
        makeReservation(12, "Middle", "2026-10-12", "11:00 AM", 3),
        makeReservation(99, "Tail", "2026-10-13", "12:00 PM", 4)
    };

    for (const Reservation& reservation : entries) {
        expect(
            system.createReservation(reservation),
            "search test creates reservation " + std::to_string(reservation.id),
            failedTests
        );
    }

    for (const Reservation& reservation : entries) {
        const Reservation* found = system.findReservationById(reservation.id);
        expect(
            found != nullptr &&
                found->id == reservation.id &&
                found->customerName == reservation.customerName &&
                found->date == reservation.date &&
                found->time == reservation.time &&
                found->partySize == reservation.partySize &&
                found->resourceId == reservation.resourceId &&
                found->studentId == reservation.studentId,
            "search returns complete " + reservation.customerName + " reservation",
            failedTests
        );
    }

    expect(system.findReservationById(777) == nullptr, "missing ID has no match", failedTests);
    expect(system.findReservationById(0) == nullptr, "zero ID has no match", failedTests);
    expect(system.findReservationById(-1) == nullptr, "negative ID has no match", failedTests);
    expect(
        system.reservationExists(40) && system.reservationExists(12) && system.reservationExists(99),
        "searches leave all reservations active",
        failedTests
    );
}

void testTailPointerAfterInsertAndRemove(int& failedTests) {
    ReservationList list;

    expect(
        list.insert(makeReservation(20, "First", "2026-09-18", "6:00 PM", 2)),
        "first list item inserts",
        failedTests
    );
    expect(
        list.insert(makeReservation(21, "Second", "2026-09-19", "7:00 PM", 3)),
        "second list item inserts",
        failedTests
    );
    expect(
        list.insert(makeReservation(22, "Third", "2026-09-20", "8:00 PM", 4)),
        "third list item inserts",
        failedTests
    );

    expect(list.remove(22), "tail item removes", failedTests);
    expect(list.findById(22) == nullptr, "removed tail cannot be found", failedTests);

    expect(
        list.insert(makeReservation(23, "Fourth", "2026-09-21", "9:00 PM", 5)),
        "insert works after tail removal",
        failedTests
    );
    expect(list.findById(23) != nullptr, "new tail can be found", failedTests);

    expect(list.remove(20), "head item removes", failedTests);
    expect(list.remove(23), "new tail removes", failedTests);
    expect(list.remove(21), "last item removes", failedTests);
    expect(list.isEmpty(), "list is empty after removals", failedTests);
}

void testCampusResourceWorkflow(int& failedTests) {
    ReservationSystem system;

    expect(
        system.addResource(1, "Study Room A", "Room", 4),
        "resource can be added",
        failedTests
    );

    Reservation first = makeReservation(30, "Ana", "2026-09-20", "10:00 AM", 3);
    first.resourceId = 1;
    first.studentId = 1001;

    expect(
        system.requestReservation(first) == ReservationRequestResult::Created,
        "available resource creates active reservation",
        failedTests
    );
    expect(!system.isResourceAvailable(1), "resource is reserved after request", failedTests);

    const Reservation* active = system.findReservationById(30);
    expect(
        active != nullptr && active->studentId == 1001 && active->resourceId == 1,
        "search finds an active campus reservation",
        failedTests
    );

    Reservation second = makeReservation(31, "Ben", "2026-09-20", "11:00 AM", 2);
    second.resourceId = 1;
    second.studentId = 1002;

    expect(
        system.requestReservation(second) == ReservationRequestResult::Waitlisted,
        "unavailable resource adds student to waiting list",
        failedTests
    );
    expect(system.waitingListSize() == 1, "waiting list stores one student", failedTests);
    expect(
        system.findReservationById(31) == nullptr,
        "waiting-list request is not returned as an active reservation",
        failedTests
    );

    expect(system.cancelReservation(30), "active reservation cancels", failedTests);
    expect(
        system.findReservationById(30) == nullptr,
        "cancelled reservation is no longer searchable",
        failedTests
    );
    expect(system.isResourceAvailable(1), "resource is available after cancellation", failedTests);
    expect(system.cancellationHistorySize() == 1, "cancellation history stores cancellation", failedTests);

    expect(
        system.processNextWaitingReservation(1, 32, "2026-09-20", "11:00 AM", 2),
        "waiting student can be assigned to available resource",
        failedTests
    );
    expect(system.waitingListSize() == 0, "waiting list is empty after processing", failedTests);
    expect(!system.isResourceAvailable(1), "resource is reserved after waiting-list processing", failedTests);

    const Reservation* processed = system.findReservationById(32);
    expect(
        processed != nullptr && processed->studentId == 1002 && processed->resourceId == 1,
        "processed waiting-list reservation is searchable",
        failedTests
    );

    expect(system.cancelReservation(32), "processed waiting reservation cancels", failedTests);
    expect(system.findReservationById(32) == nullptr, "search excludes the latest cancellation", failedTests);
    expect(system.restoreMostRecentCancellation(), "most recent cancellation restores", failedTests);
    const Reservation* restored = system.findReservationById(32);
    expect(
        restored != nullptr && restored->customerName == "Ben" && restored->partySize == 2,
        "restored reservation is searchable with its original data",
        failedTests
    );
    expect(system.cancellationHistorySize() == 1, "restore removes one history item", failedTests);
}

int main() {
    int failedTests = 0;

    testValidationEdgeCases(failedTests);
    testDuplicateIdRejectionUsesReservationSystem(failedTests);
    testReservationSearch(failedTests);
    testTailPointerAfterInsertAndRemove(failedTests);
    testCampusResourceWorkflow(failedTests);

    if (failedTests > 0) {
        std::cout << failedTests << " reservation edge case test(s) failed.\n";
        return 1;
    }

    std::cout << "Reservation edge case tests passed.\n";
    return 0;
}
