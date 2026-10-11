## Reports

The system includes a **Reports** menu that generates four read-only reports from the data the system maintains. Reports never change any data.

**How to open:** from the main menu choose **Generate reports** (option 14), then pick a report. Enter `0` to return to the main menu.

| # | Report | What it shows |
|---|--------|---------------|
| 1 | Active reservations | Every current reservation (ID, student, resource, date, time, group size) and the total count |
| 2 | Resource utilization | Each resource with its status and the number of reservations made for it, plus the total |
| 3 | Most requested resources | Resources ranked from most to least reserved (top 5), sorted with our Merge Sort |
| 4 | Waiting-list statistics | Total students waiting, the number waiting for each resource, and the longest waiting line |

**Files:** `ReportGenerator.h`, `ReportGenerator.cpp` (class `ReportGenerator`).

**Notes**
- "Reservations" in report 2 and 3 counts successful new bookings in the current program run, not only the active ones. Undo does not count a restored booking again.
- If no reservations or no waiting students exist, the report prints a clear message instead of an empty table.
- Invalid menu input (letters or numbers outside 0-4) is rejected with an error message.
