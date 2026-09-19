#ifndef WAITINGLIST_H
#define WAITINGLIST_H

#include <string>

struct WaitingEntry {
    int studentId;
    std::string studentName;
    int resourceId;
};

class WaitingList {
public:
    WaitingList();
    ~WaitingList();

    bool enqueue(int studentId, const std::string& studentName, int resourceId);

    bool dequeue(WaitingEntry& out);
    bool dequeueForResource(int resourceId, WaitingEntry& out);

    bool removeStudent(int studentId);

    bool peek(WaitingEntry& out) const;
    bool contains(int studentId) const;
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void display() const;

private:
    struct Node {
        WaitingEntry data;
        Node* next;
    };

    Node* front;
    Node* rear;
    int count;

    WaitingList(const WaitingList&);
    WaitingList& operator=(const WaitingList&);
};

#endif
