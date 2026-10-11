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
    SortMenu.cpp
    ReportGenerator.cpp
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

add_executable(resource_sort_tests test_person2_sort.cpp Resource.cpp)
target_include_directories(resource_sort_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(final_integration_tests tests/FinalIntegrationTests.cpp ${COMMON_SOURCES})
target_include_directories(final_integration_tests PRIVATE src ${CMAKE_CURRENT_SOURCE_DIR})

foreach(test_target resource_waiting_tests resource_sort_tests stack_tests)
    if(MSVC)
        target_compile_options(${test_target} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${test_target} PRIVATE -UNDEBUG)
    endif()
endforeach()

enable_testing()
add_test(NAME reservation_edge_cases COMMAND reservation_tests)
add_test(NAME resource_waiting_components COMMAND resource_waiting_tests)
add_test(NAME cancellation_stack_components COMMAND stack_tests)
add_test(NAME resource_sorting COMMAND resource_sort_tests)
add_test(NAME final_integration COMMAND final_integration_tests
    "${CMAKE_CURRENT_BINARY_DIR}/final-test-resources.txt")
set_tests_properties(resource_waiting_components resource_sorting PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_test(NAME reservation_search_menu
    COMMAND ${CMAKE_COMMAND}
        "-DAPP_PATH=$<TARGET_FILE:reservation_app>"
        "-DTEST_WORK_DIR=${CMAKE_CURRENT_BINARY_DIR}/search-menu-tests"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/ReservationSearchMenuTests.cmake"
)
set_tests_properties(reservation_search_menu PROPERTIES TIMEOUT 30)

if(CMAKE_VERSION VERSION_LESS 3.12)
    find_package(PythonInterp 3 QUIET)
    set(MENU_TEST_PYTHON "${PYTHON_EXECUTABLE}")
else()
    find_package(Python3 COMPONENTS Interpreter QUIET)
    set(MENU_TEST_PYTHON "${Python3_EXECUTABLE}")
endif()
if(MENU_TEST_PYTHON)
    execute_process(COMMAND "${MENU_TEST_PYTHON}" -c "import sys; sys.exit(0)"
        RESULT_VARIABLE PYTHON_RUN_STATUS OUTPUT_QUIET ERROR_QUIET TIMEOUT 5)
endif()
if(MENU_TEST_PYTHON AND "${PYTHON_RUN_STATUS}" STREQUAL "0")
    add_test(NAME integrated_menu
        COMMAND "${MENU_TEST_PYTHON}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/MenuIntegrationTests.py"
            "$<TARGET_FILE:reservation_app>")
    set_tests_properties(integrated_menu PROPERTIES TIMEOUT 60)
else()
    message(STATUS "Python 3 unavailable: integrated_menu test skipped; six C++/CMake suites remain enabled")
endif()
