#include "Resource.h"
#include "WaitingList.h"
#include <iostream>
#include <cassert>

int main() {
    ResourceManager rm;
    assert(rm.loadFromFile("nofile.txt") == -1);
    assert(rm.loadFromFile("resources.txt") == 5);
    rm.displayAll();
    assert(!rm.addResource(1, "Dup", "Room", 5));    
    assert(!rm.addResource(9, "", "Room", 5));
    assert(!rm.addResource(9, "X", "Room", -2));
    assert(rm.isAvailable(1));
    assert(rm.markReserved(1));
    assert(!rm.markReserved(1));     
    assert(!rm.isAvailable(1));
    assert(!rm.markReserved(99));            
    rm.displayAvailability();
    rm.displayAvailableOnly();
    assert(rm.markAvailable(1));

    WaitingList wl;
    WaitingEntry e;
    assert(!wl.dequeue(e));                         
    assert(wl.enqueue(101, "Ana", 1));
    assert(wl.enqueue(102, "Ben", 1));
    assert(wl.enqueue(103, "Cy", 1));
    assert(!wl.enqueue(102, "Ben", 1));                
    assert(!wl.enqueue(-1, "Bad", 1));              
    wl.display();
    assert(wl.removeStudent(102));              
    assert(!wl.removeStudent(999));
    assert(wl.dequeue(e) && e.studentId == 101);       
    assert(wl.removeStudent(103));                  
    assert(wl.isEmpty());
    assert(wl.enqueue(104, "Di", 2));                 
    wl.display();
    std::cout << "\nAll Person 2 tests passed.\n";
    return 0;
}
