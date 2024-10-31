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
    string sender;       // Sender of the email
    string priority;     // Priority level of the email (e.g., high, low)
    string subject;      // Subject of the email
    string content;      // Content of the email
    string dateReceived; // Date when the email was received
    string timeReceived; // Time when the email was received
    bool isSpam;
};

// Class to manage inbox using a linked list stack
class InboxManagement {
private:
    LinkedListStack<Email> inboxStack; 

    // Convert a string representation of a number to an integer
    int stringToInt(const string& str) {
        int result = 0;
        for (char c : str) {
            if (c >= '0' && c <= '9') {
                result = result * 10 + (c - '0');
            }
        }
        return result; // Return the converted integer
    }

    // Convert month string to its corresponding numerical value
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

    // Convert time string to total minutes since midnight
    int convertTimeToMinutes(const string& time) {
        size_t colonPos = time.find(':'); // Find the position of the colon
        string hourStr = time.substr(0, colonPos); // Extract hour part
        string minStr = time.substr(colonPos + 1, 2); // Extract minute part

        int hours = stringToInt(hourStr); // Convert hour to integer
        int minutes = stringToInt(minStr); // Convert minute to integer

        // Adjust for AM/PM notation
        if (time.find("PM") != string::npos && hours != 12) {
            hours += 12; // Convert PM hours to 24-hour format
        }
        else if (time.find("AM") != string::npos && hours == 12) {
            hours = 0; // Convert midnight to 0 hours
        }

        return hours * 60 + minutes; // Return total minutes
    }

    // Convert date string to an integer representation (YYYYMMDD)
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

        // Get month from the date string
        string monthStr = date.substr(i, 3);
        int month = getMonthNumber(monthStr);

        // Get year from the last 4 characters of the date string
        int year = stringToInt(date.substr(date.length() - 4));

        return year * 10000 + month * 100 + day; // Return YYYYMMDD format
    }

    // Helper function to compare two emails by their received date and time
    bool isNewer(const Email& email1, const Email& email2) {
        int date1 = convertDateToInt(email1.dateReceived);
        int date2 = convertDateToInt(email2.dateReceived);
        if (date1 != date2) {
            return date1 > date2; // Return true if email1 is newer
        }

        // If dates are equal, compare times
        int time1 = convertTimeToMinutes(email1.timeReceived);
        int time2 = convertTimeToMinutes(email2.timeReceived);
        return time1 > time2; // Return true if email1 is newer
    }

public:
    // Display the details of an email
    void displayEmail(const Email& email) {
        cout << "From: " << email.sender << endl;
        cout << "Priority: " << email.priority << endl;
        cout << "Subject: " << email.subject << endl;
        cout << "Content: " << email.content << endl;
        cout << "Date Received: " << email.dateReceived << endl;
        cout << "Time Received: " << email.timeReceived << endl;
        cout << endl;
    }

    // Push a new email onto the inbox stack in sorted order
    void pushEmail(const Email& newEmail) {
        // If the stack is empty or the new email is newer than the top email,
        // simply push it onto the stack.
        if (inboxStack.isEmpty() || isNewer(newEmail, inboxStack.getTop())) {
            inboxStack.push(newEmail);
            return;
        }

        // Otherwise, find the correct position to insert the new email
        LinkedListStack<Email> tempStack; // Temporary stack for reordering
        bool inserted = false; // Flag to check if the new email is inserted
        while (!inboxStack.isEmpty()) {
            Email currentEmail = inboxStack.getTop(); 
            inboxStack.pop(); 

            // If the new email is newer, insert it before the current email
            if (!inserted && isNewer(newEmail, currentEmail)) {
                tempStack.push(newEmail);
                inserted = true; // Mark as inserted
            }
            tempStack.push(currentEmail); // Push the current email onto temp stack    
        }

        // If the new email is still not inserted, it's the oldest, push it at the end
        if (!inserted) {
            tempStack.push(newEmail);
        }

        // Transfer the emails back to the main stack in the correct order
        while (!tempStack.isEmpty()) {
            inboxStack.push(tempStack.getTop());
            tempStack.pop();
        }
    }

    // View the most recent email
    Email viewRecentEmail() {
        if (!inboxStack.isEmpty()) {
            return inboxStack.getTop(); 
        }
        else {
            cout << "Inbox is empty!" << endl; 
            cout << endl;
        }
    }

    // Check if the inbox is empty
    bool isInboxEmpty() {
        return inboxStack.isEmpty(); 
    }

    // Remove the most recent email from the inbox
    void popRecentEmail() {
        if (!inboxStack.isEmpty()) {
            inboxStack.pop(); 
        }
        else {
            cout << "Inbox is empty!" << endl; 
            cout << endl;
        }
    }

    // Get the number of emails in the inbox
    int getInboxSize() {
        return inboxStack.getSize();
    }
};

#endif // INBOXMANAGEMENTUSINGLLSTACK_HPP
