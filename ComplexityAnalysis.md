# Complexity Analysis

Let n be active reservations, r resources, w waiting students, and h cancellation
history entries. The bounds below count data-structure operations with bounded
field lengths; copying or validating a name/time string additionally costs
O(length). Storing the complete system costs O(n + r + w + h), plus string data.

| Operation | Worst-case time | Extra space | Explanation |
| --- | --- | --- | --- |
| Linked-list append | O(1) pointer work | O(1) node plus record | tail pointer avoids walking the list; record validation/copy also reads strings |
| Reservation search | O(n) | O(1) | manual next-pointer traversal; O(1) best case at head |
| Linked-list remove | O(n) | O(1) | search by ID then unlink and repair head/tail |
| Reservation traversal/display | O(n) | O(1) | one pass |
| Create reservation | O(n + r) | O(1) node plus record | duplicate reservation scan, resource lookup, append and availability update |
| Cancel reservation | O(n + r) | O(1) history node plus record | remove reservation, push history, release resource |
| Restore cancellation | O(n + r) | O(1) reservation node plus record | duplicate/resource checks then stack pop and list append |
| Resource lookup/availability update | O(r) | O(1) | scan resource vector by ID |
| Load k resource records | O(k*r + k^2) | O(k) stored records | each accepted record checks existing/previously loaded IDs; file parsing also costs total input length |
| Waiting enqueue | O(w) | O(1) node plus record | duplicate student scan; rear-pointer append itself is O(1) |
| Waiting dequeue/front peek | O(1) | O(1) | use front pointer |
| Waiting resource peek/dequeue/count or student removal | O(w) | O(1) | scan queue until matching entry, or scan all for counts |
| Process waiting student | O(n + r + w) | O(1) node plus record | lookup, peek, validate/create, then dequeue; failed input preserves queue order |
| Stack push/peek/restore | O(1) pointer work | O(1) node for push | top pointer; record copying also costs string length |
| Stack display | O(h) | O(1) | follow history links |
| Merge Sort on resources | O(r log r) | O(r) | manual merge buffer and O(log r) recursion stack; no std::sort |
| Sorted resource copy | O(r log r) | O(r) | copy vector then Merge Sort |
| Active reservations report | O(n) | O(1) | traverse and count reservations |
| Utilization report | O(r log r) | O(r) | sorted copy by ID, then print/count |
| Most requested report | O(r log r) | O(r) | sorted copy by popularity, then print at most five |
| Waiting statistics report | O(r log r + r*w) | O(r) | sorted resource copy, plus a queue scan per resource |

Merge Sort compares numeric keys directly and lowercases temporary strings for
name/type comparisons. With unbounded text length L, name/type sorting includes
O(L) work per comparison. Equal sort keys use ascending resource ID, providing
deterministic results. Reports do not change stored resource order.

Main-menu numeric parsing reads a line of m characters in O(m) time and O(m)
temporary storage. Calendar-date validation checks the fixed YYYY-MM-DD format
and leap-year rules. New reservation counts persist after cancellation but are
not increased by undo; the utilization report is cumulative for this session.
