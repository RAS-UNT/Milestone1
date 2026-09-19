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

    Reservation second = makeReservation(31, "Ben", "2026-09-20", "11:00 AM", 2);
    second.resourceId = 1;
    second.studentId = 1002;

    expect(
        system.requestReservation(second) == ReservationRequestResult::Waitlisted,
        "unavailable resource adds student to waiting list",
        failedTests
    );
    expect(system.waitingListSize() == 1, "waiting list stores one student", failedTests);

    expect(system.cancelReservation(30), "active reservation cancels", failedTests);
    expect(system.isResourceAvailable(1), "resource is available after cancellation", failedTests);
    expect(system.cancellationHistorySize() == 1, "cancellation history stores cancellation", failedTests);

    expect(
        system.processNextWaitingReservation(1, 32, "2026-09-20", "11:00 AM", 2),
        "waiting student can be assigned to available resource",
        failedTests
    );
    expect(system.waitingListSize() == 0, "waiting list is empty after processing", failedTests);
    expect(!system.isResourceAvailable(1), "resource is reserved after waiting-list processing", failedTests);

    expect(system.cancelReservation(32), "processed waiting reservation cancels", failedTests);
    expect(system.restoreMostRecentCancellation(), "most recent cancellation restores", failedTests);
    expect(system.cancellationHistorySize() == 1, "restore removes one history item", failedTests);
}

int main() {
    int failedTests = 0;

    testValidationEdgeCases(failedTests);
    testDuplicateIdRejectionUsesReservationSystem(failedTests);
    testTailPointerAfterInsertAndRemove(failedTests);
    testCampusResourceWorkflow(failedTests);

    if (failedTests > 0) {
        std::cout << failedTests << " reservation edge case test(s) failed.\n";
        return 1;
    }

    std::cout << "Reservation edge case tests passed.\n";
    return 0;
}
