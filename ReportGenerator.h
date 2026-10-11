// ReportGenerator.h
// Declares the ReportGenerator class, which produces the four system
// reports: active reservations, resource utilization, most requested
// resources, and waiting-list statistics.
// Author: Sulav

#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "ReservationSystem.h"

class ReportGenerator {
public:
    // Lists every active reservation and the total count.
    static void activeReservations(const ReservationSystem& sys);

    // Shows each resource with the number of reservations made for it.
    static void resourceUtilization(const ReservationSystem& sys);

    // Ranks resources from most to least reserved (top N shown).
    static void mostRequested(const ReservationSystem& sys, int topN = 5);

    // Shows how many students are waiting for each resource.
    static void waitingListStatistics(const ReservationSystem& sys);

    // Menu that lets the user choose which report to view.
    static void runReportMenu(const ReservationSystem& sys);
};

#endif
