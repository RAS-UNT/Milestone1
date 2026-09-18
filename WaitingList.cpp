#include "WaitingList.h"
#include <iostream>

WaitingList::WaitingList() : front(nullptr), rear(nullptr), count(0) {}

WaitingList::~WaitingList() {
    while (front) {
        Node* t = front;
        front = front->next;
        delete t;
    }
}

bool WaitingList::enqueue(int studentId, const std::string& studentName, int resourceId) {
    if (studentId <= 0 || studentName.empty() || resourceId <= 0) return false;
    if (contains(studentId)) return false;       
    Node* n = new Node;
    n->data.studentId = studentId;
    n->data.studentName = studentName;
    n->data.resourceId = resourceId;
    n->next = nullptr;
    if (rear) rear->next = n; else front = n;
    rear = n;
    count++;
    return true;
}

bool WaitingList::dequeue(WaitingEntry& out) {
    if (!front) return false;
    Node* t = front;
    out = t->data;
    front = front->next;
    if (!front) rear = nullptr;
    delete t;
    count--;
    return true;
}

bool WaitingList::removeStudent(int studentId) {
    Node* prev = nullptr;
    Node* cur = front;
    while (cur && cur->data.studentId != studentId) { prev = cur; cur = cur->next; }
    if (!cur) return false;
    if (prev) prev->next = cur->next; else front = cur->next;
    if (cur == rear) rear = prev;
    delete cur;
    count--;
    return true;
}

bool WaitingList::peek(WaitingEntry& out) const {
    if (!front) return false;
    out = front->data;
    return true;
}

bool WaitingList::contains(int studentId) const {
    for (Node* c = front; c; c = c->next)
        if (c->data.studentId == studentId) return true;
    return false;
}

void WaitingList::display() const {
    if (!front) { std::cout << "Waiting list is empty.\n"; return; }
    std::cout << "\n=== Waiting List (" << count << ") ===\n";
    int pos = 1;
    for (Node* c = front; c; c = c->next, pos++)
        std::cout << pos << ". " << c->data.studentName << " (ID " << c->data.studentId
                  << ") waiting for resource " << c->data.resourceId << "\n";
}
