cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED APP_PATH OR NOT EXISTS "${APP_PATH}")
    message(FATAL_ERROR "APP_PATH must point to the built reservation application.")
endif()

if(NOT DEFINED TEST_WORK_DIR)
    message(FATAL_ERROR "TEST_WORK_DIR must point to a test directory in the build folder.")
endif()

file(MAKE_DIRECTORY "${TEST_WORK_DIR}")
file(WRITE "${TEST_WORK_DIR}/resources.txt" "1,Search Test Room,Room,6\n")

function(run_menu_case name input)
    cmake_parse_arguments(CASE "" "" "EXPECT;REJECT" ${ARGN})
    set(input_file "${TEST_WORK_DIR}/${name}.txt")
    file(WRITE "${input_file}" "${input}")

    execute_process(
        COMMAND "${APP_PATH}"
        WORKING_DIRECTORY "${TEST_WORK_DIR}"
        INPUT_FILE "${input_file}"
        OUTPUT_VARIABLE menu_output
        ERROR_VARIABLE menu_errors
        RESULT_VARIABLE exit_status
        TIMEOUT 10
    )

    if(NOT "${exit_status}" STREQUAL "0")
        message(FATAL_ERROR "${name} failed (${exit_status}).\n${menu_errors}\n${menu_output}")
    endif()

    foreach(expected IN ITEMS "Goodbye." ${CASE_EXPECT})
        string(FIND "${menu_output}" "${expected}" position)
        if(position EQUAL -1)
            message(FATAL_ERROR "${name} did not print '${expected}'.\n${menu_output}")
        endif()
    endforeach()

    foreach(rejected IN LISTS CASE_REJECT)
        string(FIND "${menu_output}" "${rejected}" position)
        if(NOT position EQUAL -1)
            message(FATAL_ERROR "${name} unexpectedly printed '${rejected}'.\n${menu_output}")
        endif()
    endforeach()

    message(STATUS "PASS: ${name}")
endfunction()

set(create_reservation "4\n101\n1001\nRami\n1\n2028-02-29\n10:00\n2\n")
set(invalid_id_message "Please enter a positive whole number for the reservation ID.")

run_menu_case(existing_id "${create_reservation}12\n101\n15\n"
    EXPECT "Reservation created." "Reservation #101" "Student: Rami"
        "Student ID: 1001" "Resource ID: 1" "Date: 2028-02-29"
        "Time: 10:00" "Group size: 2"
    REJECT "Reservation was not found." "${invalid_id_message}"
)

run_menu_case(missing_id "${create_reservation}12\n999\n15\n"
    EXPECT "Reservation created." "Reservation was not found."
    REJECT "Reservation #101" "${invalid_id_message}"
)

run_menu_case(empty_list "12\n101\n15\n"
    EXPECT "Reservation was not found."
    REJECT "Reservation #101" "${invalid_id_message}"
)

run_menu_case(whitespace_around_id "${create_reservation}12\n 101 \n15\n"
    EXPECT "Reservation #101"
    REJECT "${invalid_id_message}"
)

run_menu_case(cancelled_id "${create_reservation}5\n101\n12\n101\n15\n"
    EXPECT "Reservation canceled and stored in cancellation history." "Reservation was not found."
    REJECT "Reservation #101"
)

run_menu_case(restored_id "${create_reservation}5\n101\n10\n12\n101\n15\n"
    EXPECT "Most recent cancellation restored." "Reservation #101" "Student: Rami"
    REJECT "Reservation was not found."
)

foreach(invalid_id IN ITEMS "0" "-1" "abc" "101abc" "101.5" "2147483648")
    run_menu_case("invalid_${invalid_id}" "${create_reservation}12\n${invalid_id}\n12\n101\n15\n"
        EXPECT "${invalid_id_message}" "Reservation #101"
        REJECT "Reservation was not found."
    )
endforeach()

run_menu_case(blank_id "${create_reservation}12\n\n12\n101\n15\n"
    EXPECT "${invalid_id_message}" "Reservation #101"
    REJECT "Reservation was not found."
)

run_menu_case(whitespace_only_id "${create_reservation}12\n   \n12\n101\n15\n"
    EXPECT "${invalid_id_message}" "Reservation #101"
    REJECT "Reservation was not found."
)

run_menu_case(input_closes_after_search "12\n101\n"
    EXPECT "Reservation was not found."
    REJECT "${invalid_id_message}"
)

run_menu_case(input_closes_at_prompt "12\n"
    EXPECT "${invalid_id_message}"
    REJECT "Reservation was not found."
)
