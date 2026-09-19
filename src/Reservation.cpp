#include "Reservation.h"
#include <cctype>
#include <iostream>

namespace {
bool hasText(const std::string& value) {
    for (char character : value) {
        if (!std::isspace(static_cast<unsigned char>(character))) {
            return true;
        }
    }

    return false;
}

bool isDigitAt(const std::string& value, int index) {
    return std::isdigit(static_cast<unsigned char>(value[index]));
}

int readNumber(const std::string& value, int start, int length) {
    int number = 0;

    for (int index = start; index < start + length; ++index) {
        number = number * 10 + (value[index] - '0');
    }

    return number;
}

bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

bool isValidDate(const std::string& date) {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }

    for (int index = 0; index < 10; ++index) {
        if (index != 4 && index != 7 && !isDigitAt(date, index)) {
            return false;
        }
    }

    int year = readNumber(date, 0, 4);
    int month = readNumber(date, 5, 2);
    int day = readNumber(date, 8, 2);

    if (year <= 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    const int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (month == 2 && isLeapYear(year)) {
        return day <= 29;
    }

    return day <= daysInMonth[month - 1];
}
}

bool isValidReservation(const Reservation& reservation) {
    return reservation.id > 0 &&
           hasText(reservation.customerName) &&
           isValidDate(reservation.date) &&
           hasText(reservation.time) &&
           reservation.partySize > 0;
}

void displayReservation(const Reservation& reservation) {
    std::cout << "Reservation #" << reservation.id << '\n'
              << "Student: " << reservation.customerName << '\n';

    if (reservation.studentId > 0) {
        std::cout << "Student ID: " << reservation.studentId << '\n';
    }

    if (reservation.resourceId > 0) {
        std::cout << "Resource ID: " << reservation.resourceId << '\n';
    }

    std::cout
              << "Date: " << reservation.date << '\n'
              << "Time: " << reservation.time << '\n'
              << "Group size: " << reservation.partySize << '\n';
}
