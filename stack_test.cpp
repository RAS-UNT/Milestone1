#include "stack.h"
#include <iostream>
#include <cassert>

// Standalone test driver for the Stack component. Not part of the final
// integrated program — use this to demonstrate your piece works before
// it's wired into the team's main.cpp.
int main() {
    CancellationStack history;

    Reservation r1{101, "Alice Kim", "2026-09-15", "10:00", 4};
    Reservation r2{102, "Ben Lopez", "2026-09-16", "14:00", 2};
    Reservation r3{103, "Chen Wu", "2026-09-17", "09:00", 6};
    r1.resourceId = 1;
    r2.resourceId = 2;
    r3.resourceId = 3;

    std::cout << "Test 1: restore on empty stack (should fail gracefully)\n";
    Reservation temp;
    assert(!history.restore(temp));
    assert(history.isEmpty());

    std::cout << "\nTest 2: push three cancellations\n";
    history.push(r1, "2026-09-14 08:00");
    history.push(r2, "2026-09-14 09:00");
    history.push(r3, "2026-09-14 10:00");
    assert(history.size() == 3);
    assert(history.peek(temp) && temp.id == 103);
    history.display();

    std::cout << "\nTest 3: restore most recent (should be r3 / Chen Wu)\n";
    if (history.restore(temp)) {
        std::cout << "Restored: " << temp.customerName << " - " << temp.date << "\n";
    }
    history.display();

    std::cout << "\nTest 4: push again after a restore, confirm no corruption\n";
    assert(temp.id == 103 && temp.resourceId == 3);
    assert(history.size() == 2);
    history.push(r3, "2026-09-14 11:00");
    history.display();

    std::cout << "\nCurrent size: " << history.size() << "\n";
    assert(history.size() == 3);
    assert(history.restore(temp) && temp.id == 103);
    assert(history.restore(temp) && temp.id == 102);
    assert(history.restore(temp) && temp.id == 101);
    assert(history.isEmpty());
    assert(!history.restore(temp));

    return 0;
}
