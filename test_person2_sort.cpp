#include "Resource.h"
#include "Sorter.h"
#include <iostream>
#include <cassert>
#include <vector>

static bool intLess(int a, int b) { return a < b; }

int main() {
    
    std::vector<int> empty;
    Sorter::mergeSort(empty, intLess);
    assert(empty.empty());

    std::vector<int> one(1, 42);
    Sorter::mergeSort(one, intLess);
    assert(one[0] == 42);

    int raw[] = {9, 3, 7, 3, -1, 0, 12, 5, 5, 1};
    std::vector<int> v(raw, raw + 10);
    Sorter::mergeSort(v, intLess);
    for (size_t i = 1; i < v.size(); i++) assert(v[i - 1] <= v[i]);

    ResourceManager rm;
    assert(rm.loadFromFile("resources.txt") == 5);

    rm.sortResources(SORT_BY_NAME, true);
    std::vector<Resource> s = rm.sortedCopy(SORT_BY_NAME, true);
    assert(s[0].name == "Computer Lab 101");
    assert(s[4].name == "Study Room B");

    s = rm.sortedCopy(SORT_BY_CAPACITY, false);
    assert(s[0].capacity == 50 && s[4].capacity == 1);

    s = rm.sortedCopy(SORT_BY_TYPE, true);
    assert(s[0].type == "Equipment");
    assert(s[1].type == "Lab");
    assert(s[2].id == 1 && s[3].id == 2 && s[4].id == 5);

    for (int k = 0; k < 3; k++) { assert(rm.markReserved(3)); assert(rm.markAvailable(3)); }
    for (int k = 0; k < 2; k++) { assert(rm.markReserved(5)); assert(rm.markAvailable(5)); }
    assert(rm.markReserved(1));
    s = rm.sortedCopy(SORT_BY_POPULARITY, false);
    assert(s[0].id == 3 && s[0].timesReserved == 3);
    assert(s[1].id == 5 && s[1].timesReserved == 2);
    assert(s[2].id == 1 && s[2].timesReserved == 1);

    assert(rm.markAvailable(1));
    assert(rm.markReserved(1, false));
    assert(rm.findById(1)->timesReserved == 1);

    rm.sortResources(SORT_BY_POPULARITY, false);
    assert(rm.exists(4) && rm.findById(4)->name == "Projector");
    assert(!rm.isAvailable(1));
    rm.displayAll();

    rm.sortResources(SORT_BY_ID, true);
    std::vector<Resource> before = rm.sortedCopy(SORT_BY_ID, true);
    rm.sortedCopy(SORT_BY_NAME, false);
    std::vector<Resource> after = rm.sortedCopy(SORT_BY_ID, true);
    for (size_t i = 0; i < before.size(); i++) assert(before[i].id == after[i].id);

    std::cout << "\nAll Person 2 sorting tests passed.\n";
    return 0;
}
