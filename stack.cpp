#include "stack.h"
#include <iostream>

CancellationStack::CancellationStack() : top(nullptr), count(0) {}

CancellationStack::~CancellationStack() {
    while (top != nullptr) {
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }
}

// Called by Reservation Management whenever a reservation is cancelled.
void CancellationStack::push(const Reservation& res, const std::string& cancellationTime) {
    StackNode* newNode = new StackNode;
    newNode->reservation = res;
    newNode->cancellationTime = cancellationTime;
    newNode->next = top;
    top = newNode;
    count++;
}

bool CancellationStack::peek(Reservation& outReservation) const {
    if (isEmpty()) {
        return false;
    }

    outReservation = top->reservation;
    return true;
}

// Pops the most recently cancelled reservation and returns it via outReservation
// so the caller can re-insert it into the active reservations linked list.
// Returns false (and leaves outReservation untouched) if nothing to restore.
bool CancellationStack::restore(Reservation& outReservation) {
    if (isEmpty()) {
        std::cout << "No cancelled reservations to restore.\n";
        return false;
    }
    StackNode* temp = top;
    outReservation = temp->reservation;
    top = top->next;
    delete temp;
    count--;
    return true;
}

void CancellationStack::display() const {
    if (isEmpty()) {
        std::cout << "Cancellation history is empty.\n";
        return;
    }
    std::cout << "\n--- Cancellation History (most recent first) ---\n";
    StackNode* current = top;
    int position = 1;
    while (current != nullptr) {
        std::cout << position << ". Reservation #" << current->reservation.id
                   << " | " << current->reservation.customerName
                   << " | Resource " << current->reservation.resourceId
                   << " | " << current->reservation.date << " " << current->reservation.time
                   << " | Group size " << current->reservation.partySize
                   << " | Cancelled at: " << current->cancellationTime << "\n";
        current = current->next;
        position++;
    }
    std::cout << "-------------------------------------------------\n";
}

bool CancellationStack::isEmpty() const {
    return top == nullptr;
}

int CancellationStack::size() const {
    return count;
}
