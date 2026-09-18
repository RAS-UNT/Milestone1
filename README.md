# Reservation System by Badr Rami

This project is a C++ console reservation system built around a custom linked list.

## Features

- Create reservations
- Cancel reservations by ID
- Display all reservations
- Validate reservation data
- Prevent duplicate reservation IDs
- Store reservations in a linked list
- Insert, remove, traverse, and display linked list nodes

## Project Structure

- `src/main.cpp`: console menu and user input
- `src/Reservation.h` and `src/Reservation.cpp`: reservation data, validation, and display
- `src/ReservationList.h` and `src/ReservationList.cpp`: linked list implementation
- `src/ReservationSystem.h` and `src/ReservationSystem.cpp`: reservation feature layer
- `CMakeLists.txt`: CMake build configuration

## Validation Rules

A reservation must have:

- a positive reservation ID
- a non-empty customer name
- a date in `YYYY-MM-DD` format
- a non-empty time
- a party size above 0
- a reservation ID that is not already in use

## Build and Run With CMake

From the project folder:

```powershell
cmake -S . -B build
cmake --build build
.\build\reservation_app.exe
```

## Menu Options

When the program runs, choose from:

1. Create reservation
2. Cancel reservation
3. Display reservations
4. Exit

Dates should follow this format: `YYYY-MM-DD`.
