import pathlib
import subprocess
import sys
import tempfile


def main():
    app = pathlib.Path(sys.argv[1]).resolve()
    fixtures = (
        "1,Study Room A,Room,6\n"
        "2,Study Room B,Room,4\n"
        "3,Computer Lab 101,Lab,30\n"
        "4,Projector,Equipment,1\n"
        "5,Conference Hall,Room,50\n"
    )
    passed = 0
    with tempfile.TemporaryDirectory(prefix="reservation-menu-", dir=app.parent) as directory:
        root = pathlib.Path(directory)
        (root / "resources.txt").write_text(fixtures, encoding="utf-8")
        (root / "extra.txt").write_text("6,Extra Room,Room,8\n", encoding="utf-8")

        def run(name, commands, expected=(), rejected=()):
            nonlocal passed
            result = subprocess.run(
                [str(app)], input=commands, text=True, capture_output=True,
                cwd=root, timeout=10,
            )
            if result.returncode != 0:
                raise RuntimeError(f"{name}: exit {result.returncode}\n{result.stderr}\n{result.stdout}")
            for marker in ("Goodbye.", *expected):
                if marker not in result.stdout:
                    raise RuntimeError(f"{name}: missing {marker!r}\n{result.stdout}")
            for marker in rejected:
                if marker in result.stdout:
                    raise RuntimeError(f"{name}: unexpected {marker!r}\n{result.stdout}")
            print(f"PASS: {name}")
            passed += 1

        create = "4\n101\n1001\nRami\n1\n2028-02-29\n10:00\n2\n"
        invalid_id = "Please enter a positive whole number for the reservation ID."
        run("search_existing", create + "12\n101\n15\n",
            ("Reservation #101", "Student: Rami", "Student ID: 1001", "Resource ID: 1",
             "Date: 2028-02-29", "Time: 10:00", "Group size: 2"), (invalid_id,))
        run("search_missing", create + "12\n999\n15\n", ("Reservation was not found.",))
        run("search_empty", "12\n101\n15\n", ("Reservation was not found.",))
        run("search_whitespace", create + "12\n 101 \n15\n", ("Reservation #101",), (invalid_id,))
        run("search_cancelled", create + "5\n101\n12\n101\n15\n",
            ("Reservation was not found.",), ("Reservation #101",))
        run("search_restored", create + "5\n101\n10\n12\n101\n15\n",
            ("Most recent cancellation restored.", "Reservation #101"))
        for value in ("0", "-1", "abc", "101abc", "101.5", "2147483648", "", "   "):
            run("search_invalid_" + repr(value), create + f"12\n{value}\n12\n101\n15\n",
                (invalid_id, "Reservation #101"))
        run("search_eof_after", "12\n101\n", ("Reservation was not found.",))
        run("search_eof_prompt", "12\n", (invalid_id,))

        alice = "4\n1\n1001\nAlice\n4\n2026-10-11\n10:00\n1\n"
        bob = "4\n2\n1002\nBob\n4\n2026-10-11\n10:00\n1\n"
        run("reports_1_empty", "14\n1\n2\n3\n4\n0\n15\n",
            ("No active reservations.", "Total reservations made: 0",
             "No reservations have been made yet.", "Total students waiting: 0"))
        run("reports_2_created", alice + "14\n1\n2\n3\n0\n15\n",
            ("Reservation #1", "Total active reservations: 1", "Total reservations made: 1",
             "1. [4] Projector - 1 reservation(s)"))
        run("reports_3_waiting", alice + bob + "14\n4\n0\n15\n",
            ("Student added to waiting list.", "Total students waiting: 1",
             "Longest waiting line: resource 4 (1 student(s))"))
        cancelled = alice + bob + "5\n1\n"
        run("reports_4_cancelled", cancelled + "14\n1\n2\n4\n0\n15\n",
            ("No active reservations.", "Total reservations made: 1", "Total students waiting: 1"))
        processed = cancelled + "8\n4\n3\n2026-10-11\n10:00\n1\n"
        run("reports_5_processed", processed + "14\n1\n2\n4\n0\n15\n",
            ("Next waiting student was assigned to the resource.", "Reservation #3",
             "Total reservations made: 2", "Total students waiting: 0"))
        run("reports_6_invalid", "14\nabc\n9\n1abc\n0\n15\n",
            ("Invalid input. Please enter a number.", "Invalid choice. Enter a number from 0 to 4."))

        run("load_create_display_cancel_history_restore",
            "1\nextra.txt\n" + create + "6\n5\n101\n11\n10\n6\n15\n",
            ("Loaded 1 resource(s).", "Reservation created.", "Reservation #101",
             "Reservation canceled and stored in cancellation history.",
             "Cancellation History (most recent first)",
             "Most recent cancellation restored."))
        for field, name in ((1, "ID"), (2, "Name"), (3, "Type"), (4, "Capacity"), (5, "Popularity")):
            for direction, label in ((1, "ascending"), (2, "descending")):
                run(f"sort_{name}_{label}", f"13\n{field}\n{direction}\n15\n",
                    (f"Resources sorted by {name} ({label}) using Merge Sort.",))
        run("invalid_main_recovery", "15abc\nabc\n99\n15\n",
            ("Please enter a number from 1 to 15.", "Please choose a valid option."))
        run("invalid_sort_recovery", "13\nabc\n6\n2abc\n1\n3\n2\n15\n",
            ("Invalid input. Please enter a number.", "Invalid choice.", "Resources sorted by ID (descending)"))
        run("invalid_cancel_recovery", create + "5\n101abc\n12\n101\n15\n",
            ("Please enter a valid reservation ID.", "Reservation #101"))
        run("report_eof", "14\n")
        run("sort_eof", "13\n")
        run("undo_count", alice + "5\n1\n10\n14\n2\n0\n15\n",
            ("Most recent cancellation restored.", "Total reservations made: 1"),
            ("Total reservations made: 2",))
    print(f"All {passed} integrated menu cases passed.")


if __name__ == "__main__":
    main()
