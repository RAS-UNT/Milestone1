# Campus Resource Reservation System

## Description
A C++ console application that manages resource reservations. It
supports creating and cancelling reservations, validating reservation
data, maintaining a waiting list when needed, and restoring the most
recently cancelled reservation (undo). Built using CMake.

## Team Members
- **Rami** — Reservation core & Linked List: `Reservation` struct/logic,
  validation, `ReservationList` (insert/remove/traverse/display)
- **AQani** — Queue (add/remove/display), Resource Management
  (load from file, store, display, availability)
- **Sulav** — Cancellation History (Stack): push/restore/display;
  README/docs, GitHub repo & branch management, complexity analysis doc

## Features
- Create, cancel, validate, and display reservations
- Prevent duplicate reservation IDs
- Store active reservations in a linked list
- Waiting list (queue) when a resource is unavailable
- Resource loading/display from file
- Cancellation history (stack) with undo/restore


## Project Structure
```
src/
  main.cpp                  - console menu and user input
  Reservation.h / .cpp      - reservation data, validation, display
  ReservationList.h / .cpp  - linked list implementation
  ReservationSystem.h / .cpp - reservation feature layer
  stack.h / .cpp             - cancellation history (stack)
  WaitingList.h / .cpp        - waiting list (queue) 
  Resource.h / .cpp           - resource management 
tests/
  ReservationEdgeCaseTests.cpp - edge case tests 
CMakeLists.txt               - build configuration
```


## Validation Rules
A reservation must have:
- a positive reservation ID
- a non-empty customer name
- a date in `YYYY-MM-DD` format
- a non-empty time
- a party size above 0
- a reservation ID that is not already in use

## Build and Run (CMake)
From the project folder:

cmake -S . -B build
cmake --build build
.\build\reservation_app.exe


## Menu Options
1. Create reservation
2. Cancel reservation
3. Display reservations
4. Exit


## GitHub Repository
https://github.com/RAS-UNT/Milestone1
