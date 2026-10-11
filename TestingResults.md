# Final Testing Results

Date: 2026-10-11. Scope: main after merging reservation-search b6faf8e,
merge-sort bb8b3e4, and feature-reports 926cda3, plus final integration commit
578dfee (Finalize sorting and reporting integration with regression tests).

## Verified Under WSL/GCC

Environment: Ubuntu WSL2, Linux 5.15.167.4-microsoft-standard-WSL2;
GCC 13.3.0; Python 3.12.3. Compilation used C++17 with -Wall -Wextra -pedantic.
No GCC compiler warnings were emitted in this run. Component assertions were
enabled; they are also explicitly kept enabled in CMake Release builds.

Reproduce from the project root:

```bash
bash tests/run_linux_checks.sh
```

| Check | Actual result |
| --- | --- |
| Integrated app compile | Passed |
| Reservation edge-case/search tests | Passed |
| Final integration tests | Passed |
| Resource/waiting-list tests | Passed |
| Resource sorting tests | Passed |
| Stack tests | Passed |
| Scripted load/create/display/cancel/history/restore menu flow | Passed |
| Scripted integrated menu cases | Passed: all 39 |
| git diff --check | Passed |
| GCC AddressSanitizer/UndefinedBehaviorSanitizer integration run | Passed; no sanitizer diagnostics |

The Linux script builds and runs five C++ test executables and the actual
integrated application through Python subprocesses. Each menu scenario uses
an isolated resource fixture, requires normal exit, checks expected output,
and has a timeout. The app remains usable after rejected input.

## Additional Windows Check

The same source built with MSVC 19.51, CMake and Ninja in Release mode. All seven
CTest suites passed, including the original 16-case search menu test and the
39-case integrated menu test. Python 3 was explicitly configured to a working
local interpreter. MSVC emitted its existing localtime deprecation warning and
warnings about undefining NDEBUG for assertion-based test targets; GCC emitted
no compilation warnings. These Windows results are additional local checks,
not CELL execution.

## Specific Coverage

- Calendar boundaries: 2028-02-29 accepted; 2027-02-29 and 2025-02-30 rejected.
- Party sizes zero/negative rejected; capacity enforced; empty/whitespace names rejected.
- Duplicate IDs rejected through createReservation/requestReservation without replacing data.
- Linked-list head/middle/tail search and removal, append after tail removal, and empty-list cases.
- Resource loading: missing file, duplicate IDs, malformed fields, extra trailing comma,
  missing capacity, zero capacity, trailing numeric text and integer overflow.
- Waiting FIFO: busy-resource requests queued; per-resource counts correct; invalid assignment
  keeps the first student's position; successful assignment removes only that entry.
- Undo: empty history, latest-first restoration, unavailable-resource failure preserving history,
  restored record searchable, and no duplicate increment of utilization/popularity.
- Merge Sort: empty/singleton/duplicate input, key ordering, both menu directions,
  popularity, ID tie-breaking, resource lookup and availability after sorting.
- Reports: correct active/cumulative/waiting counts, most-requested ordering, empty messages,
  and no changes to active reservations, availability, waiting queue, history or booking counts.
- Menu input: blank/whitespace search IDs, zero, negative, letters, numeric suffixes,
  decimals, overflow, out-of-range choices, recovery and closed input.

## Sulav's Six Reporting Scenarios

These scenarios use the final integrated application, not a reports-only driver.
The fixture contains Study Room A, Study Room B, Computer Lab 101, Projector
(capacity 1), and Conference Hall. Scenario descriptions come from Sulav's plan.

| Scenario | Observed expected behavior | Final result |
| --- | --- | --- |
| 1. Empty reservations; run reports 1-4 | No active reservations, utilization total 0, no bookings yet, waiting total 0 | Passed, 2026-10-11 |
| 2. Alice reserves Projector with ID 1 | Reservation #1, active total 1, utilization total 1, Projector ranked first | Passed, 2026-10-11 |
| 3. Bob requests reserved Projector | Bob waitlisted; waiting total 1; longest line is resource 4 with 1 student | Passed, 2026-10-11 |
| 4. Cancel Alice's reservation | No active reservations; utilization remains 1; Bob remains waiting | Passed, 2026-10-11 |
| 5. Assign Projector to Bob with ID 3 | Reservation #3 created; waiting total 0; utilization total 2 | Passed, 2026-10-11 |
| 6. Reports input abc, 9, then 0 | Input/range errors, then return to main menu and normal exit | Passed, 2026-10-11 |

## CELL Verification

Actual UNT CSE CELL execution has NOT been performed in this session. WSL/GCC
is local Linux verification, not a substitute for the assignment's CELL check.
Run on CELL from the extracted project folder before submission:

```bash
g++ --version
bash tests/run_linux_checks.sh
./build/linux/reservation_app
```

If Python 3 is unavailable on CELL, follow README.md's direct g++ application
build and run the five C++ executables built by the script, then manually check
the menu flows. If CMake is installed, README.md also provides CMake/CTest
commands. No remote-CELL pass result is claimed here.
