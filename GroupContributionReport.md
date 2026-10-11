# Group Contribution Report

Repository: https://github.com/RAS-UNT/Project1

This is the team's single combined contribution report. Code responsibilities
are supported by the preserved branch/commit history. Reporting documentation
and the six-scenario report plan incorporate Sulav's supplied Markdown files.
Final integrated test execution and packaging were coordinated under Rami's
integration work; this does not attribute those runs separately to every member.

| Team Member | Component(s) | Specific Tasks Completed | Testing/Debugging | GitHub Contributions |
| --- | --- | --- | --- | --- |
| Rami Badr | Reservations, linked list, searching, final integration | Implemented Reservation validation; ReservationList insert/remove/findById/traverse/displayAll; optimized append with a tail pointer; implemented ReservationSystem create/request/cancel/process/restore coordination; exposed findReservationById; added strict menu input and options 12-14; added read-only report accessors and waiting counters; fixed FIFO preservation and undo booking counts; assembled final submission documents | Wrote ReservationEdgeCaseTests, 16 search-menu cases, FinalIntegrationTests, and the 39-case menu runner; verified final WSL/GCC build, component/integration suites, and load/create/display/cancel/history/restore flow; tested malformed files, overflow, invalid input, report counts, and read-only state | linked-list: 12a0ac1 through 63e9d02; reservation-search: 95fed70, d439141, 64fe9b8, b6faf8e; main: merges 044b434, c70b934, 666bff7; integration 578dfee; final documentation commit |
| Aqani (AQani2222) | Resources, waiting queue, sorting | Implemented ResourceManager file loading, storage, lookup, availability, display and reservation counters; WaitingList enqueue/dequeue/dequeueForResource/removeStudent/peek/contains/display; manual Sorter::merge/mergeSortRange/mergeSort; sortResources/sortedCopy comparators and runSortMenu | Authored test_person2.cpp for missing files, duplicates, availability and queue edge cases; authored test_person2_sort.cpp for empty/singleton/duplicate sorting, resource keys, popularity, ID tie-breaking, counter restoration and resource lookup after sorting; both passed in final integrated verification | queue: 8c7ee12, 12fd3d5, 123bcd8, 0c2be96, de72c37; merge-sort: 55c4fe7, 9e9cf11, 161e1af, e2c3b37, 5280115, bb8b3e4 |
| Sulav Bhattarai | Cancellation stack, reporting, documentation | Implemented CancellationStack push/restore/display and cleanup; ReportGenerator activeReservations/resourceUtilization/mostRequested/waitingListStatistics/runReportMenu; wrote reporting README material, user documentation, report testing plan and report complexity descriptions | Authored stack_test.cpp for empty restore, multiple pushes, restoration and reuse; prepared six reporting scenarios covering empty data, booking, waiting, cancellation, assignment and invalid menu input; final integrated verification passed those scenarios, with additional stack assertions added during integration | stack: 66a6491, 41c9768, 5fc3b81, 44410ec and README updates through 947ba62; feature-reports: adf3840, 926cda3 |

## Rami Badr

Rami implemented the reservation model and singly linked list, including
calendar-date validation, duplicate-ID checking in the system, removal at any
position, traversal/display, and manually implemented linear search. The tail
pointer reduces append pointer work from O(n) to O(1). His reservation-search
branch connects that search to the console menu and tests cancellation, undo,
waiting-list transitions, head/middle/tail matches and invalid IDs. During final
integration he preserved the three completed branches through merge commits,
connected sorting/reporting to the application and build, corrected undo
counter inflation and invalid-assignment FIFO reordering, and recorded the
final local Linux tests. He assembled the README, complete user guide, combined
contribution report, complexity analysis, test results and repository URL.

## Aqani

Aqani implemented resource management and the linked waiting queue, including
file validation, resource availability, duplicate waiting-student rejection,
FIFO removal and assignment for a selected resource. For the final milestone he
wrote the team's Merge Sort implementation and resource sorting interface for
ID, case-insensitive name/type, capacity and popularity, including a sorted-copy
operation used by the reports. He added reservation counters that distinguish
new bookings from restoration and authored the resource/queue and sorting test
drivers. Final integration reused these implementations and tests without
replacing the sorting algorithm with a library sort.

## Sulav Bhattarai

Sulav implemented the linked cancellation stack and its push, restore and
display operations, with newest-first history and destructor cleanup. For the
final milestone he implemented all four read-only reports and their submenu.
Active reporting traverses the reservation list; utilization and popularity
use a merge-sorted resource copy; waiting statistics reads per-resource queue
counts. He supplied reporting usage examples, a six-scenario testing table,
README material and report complexity analysis. Final documentation corrects
the ReportGenerator files' paths to the repository root and includes the
resource-copy sorting cost in waiting-statistics complexity.

## Verification Scope

The final build and tests were executed locally under WSL/GCC on 2026-10-11.
They are not claimed as execution on UNT CSE CELL. See TestingResults.md for
actual results and the remaining CELL verification requirement.
