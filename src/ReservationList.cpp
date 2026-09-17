#include "ReservationList.h"
#include <iostream>

ReservationList::ReservationList() : head(nullptr) {}

ReservationList::~ReservationList() {
    Node* current = head;

    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

bool ReservationList::insert(const Reservation& reservation) {
    if (!isValidReservation(reservation)) {
        return false;
    }

    Node* newNode = new Node{reservation, nullptr};

    if (head == nullptr) {
        head = newNode;
        return true;
    }

    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }

    current->next = newNode;
    return true;
}

void ReservationList::traverse(const std::function<void(const Reservation&)>& visit) const {
    Node* current = head;

    while (current != nullptr) {
        visit(current->data);
        current = current->next;
    }
}

void ReservationList::displayAll() const {
    if (isEmpty()) {
        std::cout << "No reservations found.\n";
        return;
    }

    traverse([](const Reservation& reservation) {
        displayReservation(reservation);
        std::cout << '\n';
    });
}

bool ReservationList::isEmpty() const {
    return head == nullptr;
}

