cmake_minimum_required(VERSION 3.10)

project(Milestone1ReservationSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_SOURCES
    src/Reservation.cpp
    src/ReservationList.cpp
    src/ReservationSystem.cpp
    Resource.cpp
    WaitingList.cpp
    stack.cpp
)

add_executable(reservation_app
    src/main.cpp
    ${COMMON_SOURCES}
)

target_include_directories(reservation_app PRIVATE src ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(reservation_tests
    tests/ReservationEdgeCaseTests.cpp
    ${COMMON_SOURCES}
)

target_include_directories(reservation_tests PRIVATE src ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(resource_waiting_tests
    test_person2.cpp
    Resource.cpp
    WaitingList.cpp
)

target_include_directories(resource_waiting_tests PRIVATE src ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(stack_tests
    stack_test.cpp
    stack.cpp
    src/Reservation.cpp
)

target_include_directories(stack_tests PRIVATE src ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME reservation_edge_cases COMMAND reservation_tests)
add_test(NAME resource_waiting_components COMMAND resource_waiting_tests)
add_test(NAME cancellation_stack_components COMMAND stack_tests)
