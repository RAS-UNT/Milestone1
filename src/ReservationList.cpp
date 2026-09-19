#include "ReservationList.h"
#include <iostream>

ReservationList::ReservationList() : head(nullptr), tail(nullptr) {}

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
        tail = newNode;
        return true;
    }

    tail->next = newNode;
    tail = newNode;
    return true;
}

bool ReservationList::remove(int reservationId) {
    Reservation removedReservation{};
    return remove(reservationId, removedReservation);
}

bool ReservationList::remove(int reservationId, Reservation& removedReservation) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr && current->data.id != reservationId) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        return false;
    }

    removedReservation = current->data;

    if (previous == nullptr) {
        head = current->next;
    } else {
        previous->next = current->next;
    }

    if (current == tail) {
        tail = previous;
    }

    delete current;
    return true;
}

const Reservation* ReservationList::findById(int reservationId) const {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.id == reservationId) {
            return &current->data;
        }

        current = current->next;
    }

    return nullptr;
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
