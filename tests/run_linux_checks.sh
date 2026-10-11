#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")/.."
mkdir -p build/linux
flags=(-std=c++17 -Wall -Wextra -pedantic -Isrc -I.)
common=(src/Reservation.cpp src/ReservationList.cpp src/ReservationSystem.cpp
        Resource.cpp WaitingList.cpp stack.cpp SortMenu.cpp ReportGenerator.cpp)

g++ "${flags[@]}" src/main.cpp "${common[@]}" -o build/linux/reservation_app
printf 'PASS: integrated app compile\n'
g++ "${flags[@]}" tests/ReservationEdgeCaseTests.cpp "${common[@]}" -o build/linux/reservation_tests
g++ "${flags[@]}" tests/FinalIntegrationTests.cpp "${common[@]}" -o build/linux/final_integration_tests
g++ "${flags[@]}" test_person2.cpp Resource.cpp WaitingList.cpp -o build/linux/resource_waiting_tests
g++ "${flags[@]}" test_person2_sort.cpp Resource.cpp -o build/linux/resource_sort_tests
g++ "${flags[@]}" stack_test.cpp stack.cpp src/Reservation.cpp -o build/linux/stack_tests

./build/linux/reservation_tests
./build/linux/final_integration_tests build/linux/final-test-resources.txt
./build/linux/resource_waiting_tests
./build/linux/resource_sort_tests
./build/linux/stack_tests
python3 tests/MenuIntegrationTests.py build/linux/reservation_app
printf 'PASS: all Linux checks\n'
