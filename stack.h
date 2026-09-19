#ifndef CANCELLATION_STACK_H
#define CANCELLATION_STACK_H

#include "Reservation.h"
#include <string>

// Singly linked node. Each node holds a cancelled Reservation plus the
// time it was cancelled.
struct StackNode {
    Reservation reservation;
    std::string cancellationTime;
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
    bool peek(Reservation& outReservation) const;
    bool restore(Reservation& outReservation);  // pop + hand back the data; false if empty
    void display() const;
    bool isEmpty() const;
    int size() const;
};

#endif
