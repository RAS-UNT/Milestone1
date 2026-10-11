# Campus Resource Reservation System

## Description
A C++17 console application for managing campus resource reservations. The
system loads campus resources from a file, creates and cancels reservations,
tracks active reservations with a linked list, manages waiting students with a
queue, and stores cancellation history with a stack so the most recent
cancellation can be restored.
Reservations can be searched by ID using a manually implemented linear scan.
Resources can be sorted using the team's Merge Sort, and four reports summarize
active reservations, utilization, popularity, and waiting lists.

## Team Members
- Rami: reservation core, linked-list implementation, validation, integration,
  menu, reservation ID searching, and tests.
- Aqani: resource management, file loading, resource availability, waiting-list
  queue, Merge Sort, sorting menu, and component tests.
- Sulav: cancellation history stack, restore/undo functionality, documentation,
  reporting, and complexity analysis.

## Features
- Load resources from `resources.txt`
- Display all resources and availability
- Create reservations for available campus resources
- Validate reservation IDs, student IDs, resource IDs, dates, names, times, and
  group sizes
- Reject duplicate reservation IDs
- Search active reservations by ID using linear search
- Add students to a waiting list when a requested resource is unavailable
- Process and display the waiting list
- Cancel reservations and store them in cancellation history
- Restore the most recently cancelled reservation
- Sort resources by ID, name, type, capacity, or popularity in either direction
- Generate four read-only system reports
- Run component and integration tests

## Project Structure
```text
src/
  main.cpp
  Reservation.h / Reservation.cpp
  ReservationList.h / ReservationList.cpp
  ReservationSystem.h / ReservationSystem.cpp
Resource.h / Resource.cpp
WaitingList.h / WaitingList.cpp
stack.h / stack.cpp
Sorter.h
SortMenu.h / SortMenu.cpp
ReportGenerator.h / ReportGenerator.cpp
resources.txt
tests/
  ReservationEdgeCaseTests.cpp
  ReservationSearchMenuTests.cmake
  FinalIntegrationTests.cpp
  MenuIntegrationTests.py
  run_linux_checks.sh
docs/
  ReservationSearch.md
test_person2.cpp
test_person2_sort.cpp
stack_test.cpp
CMakeLists.txt
UserDocumentation.md
GroupContributionReport.md
ComplexityAnalysis.md
TestingResults.md
GitHubRepository.txt
```

## Build and Run on Linux/CELL
From the project folder:

```bash
cmake -S . -B build
cmake --build build
./build/reservation_app
```

Run the tests:

```bash
ctest --test-dir build --output-on-failure
```

CTest runs six suites: reservation edge cases, resource/waiting-list components,
cancellation stack components, resource sorting, final integration, and 16
reservation-search menu cases. If a working Python 3 interpreter is available,
a seventh suite runs 39 integrated menu cases. Assertions remain enabled in
component tests even for Release builds.

If CMake is not available, compile directly with `g++`:

```bash
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pedantic -Isrc -I. \
  src/main.cpp src/Reservation.cpp src/ReservationList.cpp \
  src/ReservationSystem.cpp Resource.cpp WaitingList.cpp stack.cpp \
  SortMenu.cpp ReportGenerator.cpp \
  -o build/reservation_app
./build/reservation_app
```

To compile the application and run all five C++ test executables plus the 39
scripted menu cases without CMake, install/use GCC and Python 3, then run:

```bash
bash tests/run_linux_checks.sh
```

The final integrated source was verified on 2026-10-11 under WSL/GCC 13.3.0.
This is a local Linux check, not verification on UNT CSE CELL. The assignment's
actual CELL check remains required. See [Testing Results](TestingResults.md).

## Menu Options
1. Load resources from file
2. Display all resources
3. Display resource availability
4. Create reservation
5. Cancel reservation
6. Display active reservations
7. Add student to waiting list
8. Process next waiting student
9. Display waiting list
10. Restore most recent cancellation
11. Display cancellation history
12. Search reservation by ID
13. Sort resources
14. Generate reports
15. Exit

## Reservation Search
Choose option 12 and enter a positive integer reservation ID. The program displays
the matching active reservation or prints `Reservation was not found.` Surrounding
whitespace is accepted; blank input, nonpositive IDs, letters, decimals, and values
outside the `int` range are rejected.

Cancelled reservations are excluded from searches. Restoring a cancellation with
option 10 makes that reservation searchable again. Waiting-list requests become
searchable after they are converted into active reservations.

See [Reservation Search Notes](docs/ReservationSearch.md) for the algorithm,
complexity, recorded test results, CELL verification commands, and Rami's search
contribution notes.

## Sorting and Reports
Option 13 opens the sorting menu. Choose ID, name, type, capacity, or popularity,
then ascending or descending. Name/type comparisons are case-insensitive; ties
use ascending resource ID. Sorting changes resource display order, not bookings.

Option 14 opens Reports; select 1 for active reservations, 2 for resource
utilization, 3 for most requested resources, or 4 for waiting-list statistics.
Enter 0 to return. Reports sort copies and do not modify application data.
Utilization/popularity count successful new bookings during this run, including
bookings later cancelled; undo restores a booking without counting it again.

All data except the resource input file is held in memory and resets on exit.
Each resource is either available or reserved, independent of its date/time;
the program does not implement a calendar of separate time slots.

See [User Documentation](UserDocumentation.md),
[Complexity Analysis](ComplexityAnalysis.md), and the single
[Group Contribution Report](GroupContributionReport.md).

## GitHub Repository
https://github.com/RAS-UNT/Project1
