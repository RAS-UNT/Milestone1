# Reservation Search Notes

This document covers the reservation-search work on the `reservation-search`
branch. Its contribution notes can be incorporated into the team's final
contribution report and complexity analysis.

## User Guide

Run the application from the project folder so it can load `resources.txt`.
Create a reservation with option 4, then select option 12 to search by its ID.
Option 15 exits the program; options 13 and 14 open sorting and reports.

An existing ID displays the reservation ID, student name, student ID, resource
ID, date, time, and group size. An unknown or cancelled ID prints
`Reservation was not found.` Option 10 restores the most recent cancellation,
making that reservation searchable again. Waiting-list requests are searchable
after they have been processed into active reservations.

The search ID must be a positive integer that fits in a C++ `int`. Input such as
`101` or ` 101 ` is accepted. Blank or whitespace-only input, `0`, `-1`, `abc`,
`101abc`, `101.5`, and an overflowing integer are rejected with
`Please enter a positive whole number for the reservation ID.` The user can
then enter another menu choice. Closing input ends the program.

## Algorithm and Complexity

The existing [ReservationList::findById()](../src/ReservationList.cpp) implements
linear search manually. It starts at the head, compares each reservation's ID
with the requested ID, and follows the next pointer until it finds a match or
reaches the end. It scans the current list order without requiring sorted data.

[ReservationSystem::findReservationById()](../src/ReservationSystem.cpp) exposes
that search to the menu. `reservationExists()` also calls this method when
checking for duplicate IDs. The menu displays a match through the existing
`displayReservation()` function.

Let `n` be the number of active reservations.

| Operation | Best Time | Worst Time | Extra Search Space |
| --- | --- | --- | --- |
| `ReservationList::findById()` | O(1) | O(n) | O(1) |
| `ReservationSystem::findReservationById()` | O(1) | O(n) | O(1) |
| `ReservationSystem::reservationExists()` | O(1) | O(n) | O(1) |

A match at the head takes one comparison. A match at the tail or a missing ID
can require examining all `n` reservations. An empty list returns immediately.
Search uses a pointer to an existing reservation and does not allocate or copy
the list. Use the returned pointer immediately; cancellation of that reservation
or destruction of the system invalidates it.

The menu separately reads and parses an input line of `m` characters. That input
handling takes O(m) time and O(m) temporary storage.

## Recorded Test Results

Local verification on 2026-10-11 used a C++17 MSVC Release build with CMake and
Ninja. The tested source includes search commits through `64fe9b8`. The build
succeeded, and all four registered CTest suites passed.

That initial result predates final integration. The final merged source also
passed all five C++ executables and 39 menu cases under WSL/GCC 13.3.0, including
these same 16 search scenarios. See [Final Testing Results](../TestingResults.md)
for the current build and verification status.

| Test Suite | Local Result |
| --- | --- |
| `reservation_edge_cases` | Passed |
| `resource_waiting_components` | Passed |
| `cancellation_stack_components` | Passed |
| `reservation_search_menu` | Passed: all 16 cases |

The search assertions in
[ReservationEdgeCaseTests.cpp](../tests/ReservationEdgeCaseTests.cpp) cover:

- An empty list, missing ID `777`, zero, and negative IDs.
- Complete record matches at the head, middle, and tail using unsorted IDs `40`,
  `12`, and `99`.
- Searches leaving reservations active and retaining the original record after
  duplicate ID rejection.
- Active campus reservations, exclusion of waiting-list requests, and lookup
  after waiting-list processing.
- Lookup after cancellation and restoration, including restored record data.

[ReservationSearchMenuTests.cmake](../tests/ReservationSearchMenuTests.cmake)
runs these 16 cases through the actual application:

| Case | Expected Behavior | Local Result |
| --- | --- | --- |
| Existing ID `101` | Display the matching reservation's complete details | Passed |
| Missing ID `999` | Display the not-found message | Passed |
| Empty reservation list | Display the not-found message | Passed |
| ID ` 101 ` | Accept surrounding whitespace and display the match | Passed |
| Cancelled ID `101` | Display the not-found message | Passed |
| Restored ID `101` | Display the restored reservation | Passed |
| ID `0` | Reject input, then accept a valid search | Passed |
| ID `-1` | Reject input, then accept a valid search | Passed |
| ID `abc` | Reject input, then accept a valid search | Passed |
| ID `101abc` | Reject trailing text, then accept a valid search | Passed |
| ID `101.5` | Reject a decimal, then accept a valid search | Passed |
| ID `2147483648` | Reject integer overflow, then accept a valid search | Passed |
| Blank ID | Reject input, then accept a valid search | Passed |
| Whitespace-only ID | Reject input, then accept a valid search | Passed |
| Input closes after search | Print the result and exit without looping | Passed |
| Input closes at the ID prompt | Handle missing input and exit without looping | Passed |

Menu tests create their own resource and input files under the build folder.
The recorded local run used `build/search-step2`; its generated test log is
`build/search-step2/Testing/Temporary/LastTest.log`.

## Run on CELL

CELL verification is pending. Run these commands from the project folder on
CELL, then record the machine/compiler details and actual output in the team's
final test results.

```bash
cmake -S . -B build
cmake --build build
cd build
ctest --output-on-failure
cd ..
./build/reservation_app
```

For only the search menu tests, run from the project folder:

```bash
cd build
ctest -R '^reservation_search_menu$' --output-on-failure
cd ..
```

## Contribution Report Notes

This excerpt covers Rami's searching work. His earlier linked-list and
reservation work should also be represented in the team's full report.

| Team Member | Component(s) | Specific Tasks Completed | Testing/Debugging | GitHub Contributions |
| --- | --- | --- | --- | --- |
| Rami | Reservation search, menu integration, search documentation | Added `findReservationById()`, reused it in `reservationExists()`, added menu option 12 and strict ID validation, handled closed input, and documented usage and complexity | Added C++ search assertions and 16 automated menu cases; built locally and verified four passing CTest suites; CELL verification pending | `reservation-search`: `95fed70`, `d439141`, `64fe9b8` |

Rami's search contribution connects his existing manually implemented linked-list
linear search to the unified reservation system and console menu. The menu
rejects invalid IDs and displays matching active reservations. Added tests check
search results, input recovery, cancellation, restoration, and waiting-list
transitions. This document records the O(n) worst-case search time, O(1) extra
search space, and verified local results; the README documents the updated menu.

Search implementation and test commits recorded so far:

- `95fed70`: `Step #1: added reservation search access`
- `d439141`: `Step #2: added reservation search menu option`
- `64fe9b8`: `Step #3: added reservation search tests`
