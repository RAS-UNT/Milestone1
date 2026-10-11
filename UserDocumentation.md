# User Documentation

## Starting the Application

Use a C++17 compiler. Run the build commands in README.md from the project root,
then start `./build/reservation_app` (or `./build/linux/reservation_app` after
`bash tests/run_linux_checks.sh`). Run from the folder containing resources.txt.
The program automatically loads its five sample resources on startup.

## Resource File

Each line contains `id,name,type,capacity`, for example:

```text
1,Study Room A,Room,6
4,Projector,Equipment,1
```

IDs and capacities must be positive whole numbers within the C++ int range.
Names and types cannot be blank. Surrounding whitespace is trimmed. Blank lines
and lines beginning with # are ignored. Malformed or duplicate records produce
warnings and are skipped. Quoted comma-containing fields are not supported.

Option 1 loads another file into the existing resources; it does not clear
existing resources or reservations. A missing file reports an error and leaves
the existing data intact. Options 2 and 3 display resources and availability.

## Creating, Searching, and Cancelling

Select option 4 and provide reservation ID, student ID, student name, resource
ID, date, time, and group size. IDs must be positive whole numbers. Reservation
IDs must be unique among active reservations. Dates must be real calendar dates
in YYYY-MM-DD format. Names and times must contain non-whitespace text. Time is
a text label; HH:MM syntax is not enforced. Group size must be positive and no
larger than the resource's capacity.

A request for an available resource creates a reservation. A valid request for
a reserved resource adds the student to the waiting list instead. A student
cannot appear twice in the waiting list. The waiting queue stores the student
and requested resource, not the original reservation ID, date, time, or size.

Option 6 displays active reservations. Option 12 searches an active reservation
by ID and displays its complete record, or `Reservation was not found.` Search
accepts surrounding whitespace but rejects blank input, nonpositive values,
decimals, trailing text, and integer overflow. Cancelled and waiting requests
are not active search results.

Select option 5 and enter an active reservation ID to cancel it. Cancellation
releases its resource and stores the reservation in cancellation history.
Cancelling an unknown ID does not change data.

## Waiting Lists and Undo

Option 7 manually adds a student to the waiting queue after entering a positive
student ID, nonblank name, and existing resource ID. Option 9 displays the queue.

Option 8 assigns the first student waiting for a chosen available resource.
Provide the resource ID, a new reservation ID, date, time, and group size. Other
resources' waiting entries are skipped without changing their order. An invalid
assignment leaves the student in the same position. Cancellation alone does not
automatically process a waiting student; use option 8 afterward.

Option 11 displays cancellations, newest first. Option 10 restores the most
recent cancelled reservation. Restoration fails if its ID is already active or
its resource is reserved. A failed restoration keeps the history entry, and a
successful restoration makes the reservation searchable again. Undo is LIFO,
not a selection of arbitrary cancellations.

## Sorting

Choose option 13, then 1 for ID, 2 for name, 3 for type, 4 for capacity, or 5 for
popularity. Choose 1 for ascending or 2 for descending. Enter 0 at the field menu
to go back. Name/type comparisons ignore letter case. Equal keys are ordered by
ascending resource ID in either direction. Sorting changes display order but
keeps availability, booking counts, and reservations intact.

## Reports

Choose Generate reports (option 14), then a report numbered 1-4. The report menu
reappears after each report. Enter 0 to return to the main menu. Invalid input
prints an error and lets you try again. These reporting descriptions incorporate
Sulav's supplied user documentation.

| Report | Output |
| --- | --- |
| 1. Active reservations | Every active reservation with ID, student, resource, date, time, and group size, followed by the total count |
| 2. Resource utilization | Every resource sorted by ID, its available/reserved status, successful new-booking count, and the total |
| 3. Most requested resources | Up to five resources ordered by highest booking count, with ties resolved by ascending ID |
| 4. Waiting-list statistics | Total waiting students, counts per resource, and the resource with the longest line |

Resource utilization and popularity are cumulative within the current session:
cancelled bookings still count, waitlisted requests do not count until assigned,
and undo does not count the same booking again. If some resource has been booked,
the top-five report can also include zero-count resources at the end.

Empty reports show clear messages such as `No active reservations.`,
`No reservations have been made yet.`, or `No students are currently waiting.`
Reports read existing data or sort a copy; they do not change resource order,
reservations, waiting students, availability, or cancellation history.

For example, reserve the Projector (resource 4) for Alice, then request it for
Bob. Active reservations shows Alice; waiting statistics shows Bob waiting for
resource 4. Cancel Alice, then process Bob through option 8. Utilization now shows
two successful bookings and zero waiting students.

## Exiting and Troubleshooting

Option 15 exits. Closing standard input also exits cleanly. Reservations,
waiting lists, cancellation history, and booking counts are not saved to disk.
Resource availability is global, not scheduled by date/time.

| Problem | Action |
| --- | --- |
| Cannot open resources.txt | Run from the project root or load the correct path with option 1 |
| Invalid numeric input | Enter one whole number, without decimals or trailing text |
| Group exceeds capacity | Use a smaller group or choose another resource |
| Cannot process a waiting student | Check resource availability, waiting entries, new ID, date, time, and size |
| No cancellation could be restored | Check that history is nonempty, the resource is available, and the ID is not active |

Build instructions are in README.md; verified test results are in TestingResults.md.
