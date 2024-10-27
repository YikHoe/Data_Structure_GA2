#pragma once
#ifndef INBOXMANAGEMENTUSINGLLSTACK_HPP
#define INBOXMANAGEMENTUSINGLLSTACK_HPP
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "LinkedListStack.hpp"
using namespace std;

struct Email {
    string sender;
    string priority;
    string subject;
    string content;
    string dateReceived;
    string timeReceived;
};

class InboxManagement {
private:
    LinkedListStack<Email> inboxStack;

    int stringToInt(const string& str) {
        int result = 0;
        for (char c : str) {
            if (c >= '0' && c <= '9') {
                result = result * 10 + (c - '0');
            }
        }
        return result;
    }

    int getMonthNumber(const string& month) {
        if (month == "Jan") return 1;
        if (month == "Feb") return 2;
        if (month == "Mar") return 3;
        if (month == "Apr") return 4;
        if (month == "May") return 5;
        if (month == "Jun") return 6;
        if (month == "Jul") return 7;
        if (month == "Aug") return 8;
        if (month == "Sep") return 9;
        if (month == "Oct") return 10;
        if (month == "Nov") return 11;
        if (month == "Dec") return 12;
        return 0;
    }

    int convertTimeToMinutes(const string& time) {
        size_t colonPos = time.find(':');
        string hourStr = time.substr(0, colonPos);
        string minStr = time.substr(colonPos + 1, 2);

        int hours = stringToInt(hourStr);
        int minutes = stringToInt(minStr);

        if (time.find("PM") != string::npos && hours != 12) {
            hours += 12;
        }
        else if (time.find("AM") != string::npos && hours == 12) {
            hours = 0;
        }

        return hours * 60 + minutes;
    }

    int convertDateToInt(const string& date) {
        // Extract day
        int day = 0;
        int i = 0;
        while (i < date.length() && isdigit(date[i])) {
            day = day * 10 + (date[i] - '0');
            i++;
        }

        // Skip "th", "st", "nd", "rd" and space
        while (i < date.length() && !isupper(date[i])) {
            i++;
        }

        // Get month
        string monthStr = date.substr(i, 3);
        int month = getMonthNumber(monthStr);

        // Get year (last 4 characters)
        int year = stringToInt(date.substr(date.length() - 4));

        return year * 10000 + month * 100 + day;
    }

    // Helper function to compare two emails by date and time
    bool isNewer(const Email& email1, const Email& email2) {
        int date1 = convertDateToInt(email1.dateReceived);
        int date2 = convertDateToInt(email2.dateReceived);
        if (date1 != date2) {
            return date1 > date2;
        }

        // If dates are equal, compare times
        int time1 = convertTimeToMinutes(email1.timeReceived);
        int time2 = convertTimeToMinutes(email2.timeReceived);
        return time1 > time2;
    }

public:
    void displayEmail(const Email& email) {
        cout << "From: " << email.sender << endl;
        cout << "Priority: " << email.priority << endl;
        cout << "Subject: " << email.subject << endl;
        cout << "Content: " << email.content << endl;
        cout << "Date Received: " << email.dateReceived << endl;
        cout << "Time Received: " << email.timeReceived << endl;
        cout << endl;
    }

    void pushEmail(const Email& newEmail) {
        // If the stack is empty or the new email is newer than the top email,
        // simply push it onto the stack.
        if (inboxStack.isEmpty() || isNewer(newEmail, inboxStack.getTop())) {
            inboxStack.push(newEmail);
            return;
        }

        // Otherwise, find the correct position to insert the new email
        LinkedListStack<Email> tempStack;
        bool inserted = false;
        while (!inboxStack.isEmpty()) {
            Email currentEmail = inboxStack.getTop();
            inboxStack.pop();

            if (!inserted && isNewer(newEmail, currentEmail)) {
                tempStack.push(newEmail);
                inserted = true;
            }
            tempStack.push(currentEmail);    
        }

        // If the new email is still not inserted, it's the oldest, push it at the end
        if (!inserted) {
            tempStack.push(newEmail);
        }

        // Transfer the emails back to the main stack
        while (!tempStack.isEmpty()) {
            inboxStack.push(tempStack.getTop());
            tempStack.pop();
        }
    }

    Email viewRecentEmail() {
        if (!inboxStack.isEmpty()) {
            return inboxStack.getTop();
        }
        else {
            cout << "Inbox is empty!" << endl;
            cout << endl;
        }
    }

    bool isInboxEmpty() {
        return inboxStack.isEmpty();
    }

    void popRecentEmail() {
        if (!inboxStack.isEmpty()) {
            inboxStack.pop();
        }
        else {
            cout << "Inbox is empty!" << endl;
            cout << endl;
        }
    }
};

#endif // INBOXMANAGEMENTUSINGLLSTACK_HPP