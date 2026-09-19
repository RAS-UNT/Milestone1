cmake_minimum_required(VERSION 3.10)

project(Milestone1ReservationSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(reservation_app
    src/main.cpp
    src/Reservation.cpp
    src/ReservationList.cpp
    src/ReservationSystem.cpp
)

target_include_directories(reservation_app PRIVATE src)

add_executable(reservation_tests
    tests/ReservationEdgeCaseTests.cpp
    src/Reservation.cpp
    src/ReservationList.cpp
    src/ReservationSystem.cpp
)

target_include_directories(reservation_tests PRIVATE src)

enable_testing()
add_test(NAME reservation_edge_cases COMMAND reservation_tests)
