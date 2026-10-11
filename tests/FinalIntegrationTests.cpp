#include "ReservationSystem.h"
#include "ReportGenerator.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    int failures = 0;
    auto expect = [&failures](bool condition, const std::string& name) {
        if (!condition) {
            std::cerr << "FAIL: " << name << '\n';
            ++failures;
        }
    };

    const std::string filename = argc > 1 ? argv[1] : "build/final-test-resources.txt";
    {
        std::ofstream file(filename);
        file << "# Test resources\n\n"
             << "1, Room A ,Room,6\n"
             << "2,Projector,Equipment,1\n"
             << "1,Duplicate,Room,3\n"
             << "3,Overflow,Room,4294967297\n"
             << "4294967297,Overflow ID,Room,5\n"
             << "4,Extra comma,Room,5,\n"
             << "5,Missing capacity,Room,\n"
             << "6,Zero,Room,0\n"
             << "7,Numeric suffix,Room,5abc\n"
             << "8, ,Room,5\n";
        expect(file.good(), "write resource fixture");
    }
    ResourceManager loader;
    expect(loader.loadFromFile(filename) == 2, "file rejects malformed, duplicate, overflow records");
    expect(loader.count() == 2 && loader.findById(1)->name == "Room A", "file trims fields");
    expect(loader.loadFromFile(filename + ".missing") == -1, "missing file rejected");
    expect(loader.count() == 2, "missing file preserves existing resources");
    std::remove(filename.c_str());

    ReservationSystem system;
    expect(system.addResource(1, "Study Room", "Room", 6), "add study room");
    expect(system.addResource(4, "Projector", "Equipment", 1), "add projector");
    Reservation alice{1, "Alice", "2026-10-11", "10:00", 1, 4, 1001};
    Reservation bob{2, "Bob", "2026-10-11", "10:00", 1, 4, 1002};
    expect(system.requestReservation(alice) == ReservationRequestResult::Created, "create booking");
    expect(system.requestReservation(alice) == ReservationRequestResult::DuplicateReservation,
           "duplicate create rejected");
    expect(system.getResources().findById(4)->timesReserved == 1, "creation counted once");
    expect(system.requestReservation(bob) == ReservationRequestResult::Waitlisted, "busy resource waitlists");
    expect(system.addToWaitingList(1003, "Chen", 4), "second waiting student");
    expect(!system.addToWaitingList(1004, "   \t", 4), "whitespace waiting name rejected");
    expect(system.getWaitingList().countForResource(4) == 2, "per-resource waiting count");
    expect(system.getWaitingList().countForResource(1) == 0, "unused resource waiting count");
    expect(system.cancelReservation(1), "cancel active booking");
    expect(!system.reservationExists(1) && system.isResourceAvailable(4), "cancel releases resource");
    expect(system.getResources().findById(4)->timesReserved == 1, "cancel preserves cumulative count");
    expect(!system.processNextWaitingReservation(4, 3, "2027-02-29", "10:00", 1), "invalid wait date rejected");
    expect(!system.processNextWaitingReservation(4, 3, "2028-02-29", "10:00", 0), "zero wait group rejected");
    expect(!system.processNextWaitingReservation(4, 3, "2028-02-29", "10:00", -1), "negative wait group rejected");
    WaitingEntry waiting{};
    expect(system.getWaitingList().peekForResource(4, waiting) && waiting.studentId == 1002,
           "failed assignment preserves FIFO head");
    expect(system.waitingListSize() == 2, "failed assignment preserves waiting count");
    expect(system.processNextWaitingReservation(4, 3, "2028-02-29", "10:00", 1), "process first waiting student");
    const Reservation* processed = system.findReservationById(3);
    expect(processed && processed->studentId == 1002, "first student receives resource");
    expect(system.getResources().findById(4)->timesReserved == 2, "waiting booking increments count");
    expect(!system.restoreMostRecentCancellation(), "undo blocked by busy resource");
    expect(system.cancellationHistorySize() == 1, "failed undo preserves history");
    expect(system.cancelReservation(3), "cancel processed booking");
    expect(system.restoreMostRecentCancellation(), "undo restores latest booking");
    expect(system.findReservationById(3) && !system.reservationExists(1), "undo uses LIFO order");
    expect(system.getResources().findById(4)->timesReserved == 2, "undo does not inflate popularity");

    std::ostringstream captured;
    std::streambuf* original = std::cout.rdbuf(captured.rdbuf());
    ReportGenerator::activeReservations(system);
    ReportGenerator::resourceUtilization(system);
    ReportGenerator::mostRequested(system);
    ReportGenerator::waitingListStatistics(system);
    std::cout.rdbuf(original);
    const std::string reports = captured.str();
    expect(reports.find("Total active reservations: 1") != std::string::npos, "active report count");
    expect(reports.find("Total reservations made: 2") != std::string::npos, "utilization cumulative count");
    expect(reports.find("1. [4] Projector - 2 reservation(s)") != std::string::npos, "popularity report order");
    expect(reports.find("Longest waiting line: resource 4 (1 student(s))") != std::string::npos,
           "waiting report counts");
    expect(system.reservationExists(3) && !system.isResourceAvailable(4) &&
           system.waitingListSize() == 1 && system.cancellationHistorySize() == 1 &&
           system.getResources().findById(4)->timesReserved == 2, "all reports leave state unchanged");

    expect(system.cancelReservation(3) && system.processNextWaitingReservation(4, 5, "2028-02-29", "11:00", 1),
           "second waiting student processed");
    expect(system.findReservationById(5) && system.findReservationById(5)->studentId == 1003,
           "FIFO second student correct");
    expect(system.waitingListSize() == 0, "waiting queue empties");

    if (failures != 0) return 1;
    std::cout << "All final integration tests passed.\n";
    return 0;
}
