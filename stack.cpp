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
    newNode->data.reservation = res;
    newNode->data.cancellationTime = cancellationTime;
    newNode->next = top;
    top = newNode;
    count++;
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
    outReservation = temp->data.reservation;
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
        std::cout << position << ". Reservation #" << current->data.reservation.reservationID
                   << " | " << current->data.reservation.studentName
                   << " | " << current->data.reservation.resourceName
                   << " | Cancelled at: " << current->data.cancellationTime << "\n";
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
