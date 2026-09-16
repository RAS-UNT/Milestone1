#include "stack.h"
#include <iostream>

// Standalone test driver for the Stack component. Not part of the final
// integrated program — use this to demonstrate your piece works before
// wiring it into the team's main.cpp.
int main() {
    CancellationStack history;

    Reservation r1{101, "Alice Kim", "Study Room A", 5, "2026-09-15 10:00", "2026-09-15 11:00"};
    Reservation r2{102, "Ben Lopez", "Projector 3", 12, "2026-09-16 14:00", "2026-09-16 15:00"};
    Reservation r3{103, "Chen Wu", "Study Room B", 6, "2026-09-17 09:00", "2026-09-17 10:00"};

    std::cout << "Test 1: restore on empty stack (should fail gracefully)\n";
    Reservation temp;
    history.restore(temp);

    std::cout << "\nTest 2: push three cancellations\n";
    history.push(r1, "2026-09-14 08:00");
    history.push(r2, "2026-09-14 09:00");
    history.push(r3, "2026-09-14 10:00");
    history.display();

    std::cout << "\nTest 3: restore most recent (should be r3 / Chen Wu)\n";
    if (history.restore(temp)) {
        std::cout << "Restored: " << temp.studentName << " - " << temp.resourceName << "\n";
    }
    history.display();

    std::cout << "\nTest 4: push again after a restore, confirm no corruption\n";
    history.push(r3, "2026-09-14 11:00");
    history.display();

    std::cout << "\nCurrent size: " << history.size() << "\n";

    return 0;
}
