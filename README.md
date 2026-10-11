# Campus Resource Reservation System

## Description
A C++17 console application for managing campus resource reservations. The
system loads campus resources from a file, creates and cancels reservations,
tracks active reservations with a linked list, manages waiting students with a
queue, and stores cancellation history with a stack so the most recent
cancellation can be restored.
Reservations can also be searched by ID using a manually implemented linear scan.

## Team Members
- Rami: reservation core, linked-list implementation, validation, integration,
  menu, reservation ID searching, and tests.
- AQani: resource management, file loading, resource availability, waiting-list
  queue, and component tests.
- Sulav: cancellation history stack, restore/undo functionality, documentation,
  and complexity analysis.

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
resources.txt
tests/
  ReservationEdgeCaseTests.cpp
  ReservationSearchMenuTests.cmake
docs/
  ReservationSearch.md
test_person2.cpp
stack_test.cpp
CMakeLists.txt
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
cd build
ctest --output-on-failure
cd ..
```

CTest runs four suites: reservation edge cases (including search checks),
resource/waiting-list components, cancellation stack components, and 16
automated reservation-search menu cases. Menu tests use generated fixtures
inside the build folder.

If CMake is not available, compile directly with `g++`:

```bash
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pedantic -Isrc -I. \
  src/main.cpp src/Reservation.cpp src/ReservationList.cpp \
  src/ReservationSystem.cpp Resource.cpp WaitingList.cpp stack.cpp \
  -o build/reservation_app
./build/reservation_app
```

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
13. Exit

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

## GitHub Repository
https://github.com/RAS-UNT/Milestone1
