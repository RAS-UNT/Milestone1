#ifndef RESERVATION_LIST_H
#define RESERVATION_LIST_H
#include "Reservation.h"
#include <functional>

class ReservationList {
public:
    ReservationList();
    ~ReservationList();
    ReservationList(const ReservationList& other) = delete;
    ReservationList& operator=(const ReservationList& other) = delete;

    bool insert(const Reservation& reservation);
    bool remove(int reservationId);
    const Reservation* findById(int reservationId) const;
    void traverse(const std::function<void(const Reservation&)>& visit) const;
    void displayAll() const;
    bool isEmpty() const;

private:
    struct Node {
        Reservation data;
        Node* next;
    };

    Node* head;
    Node* tail;
};

#endif
