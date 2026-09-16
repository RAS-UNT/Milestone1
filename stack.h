#ifndef CANCELLATION_STACK_H
#define CANCELLATION_STACK_H

#include "reservation.h"

// Singly linked node — the stack is linked-list-backed so it has no fixed
// size limit and matches the style of the rest of the project.
struct StackNode {
    CancelledReservation data;
    StackNode* next;
};

class CancellationStack {
private:
    StackNode* top;
    int count;

public:
    CancellationStack();
    ~CancellationStack();               // frees any remaining nodes

    void push(const Reservation& res, const std::string& cancellationTime);
    bool restore(Reservation& outReservation);  // pop + hand back the data; false if empty
    void display() const;
    bool isEmpty() const;
    int size() const;
};

#endif
