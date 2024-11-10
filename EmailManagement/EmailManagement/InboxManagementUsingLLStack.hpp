#pragma once
#ifndef INBOXMANAGEMENTUSINGLLSTACK_HPP
#define INBOXMANAGEMENTUSINGLLSTACK_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "LinkedListStack.hpp"
#include "spamDetector.hpp"

using namespace std;

struct Email {
	string sender;       // Sender of the email
	string priority;     // Priority level of the email (e.g., high, low)
	string subject;      // Subject of the email
	string content;      // Content of the email
	string dateReceived; // Date when the email was received
	string timeReceived; // Time when the email was received
	string receiver;     // Receiver
	bool isSpam;         // Spam flag
};

// Class to manage inbox using a linked list stack
class InboxManagement {
private:
	LinkedListStack<Email> inboxStack;
	string stackName;

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
	InboxManagement(string stackName) {
		this->stackName = stackName;
	}

	~InboxManagement() {
		cout << stackName << " has been deleted from memory" << endl;
	}

	// Display the details of an email
	void displayEmail(const Email& email) {
		cout << endl;
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

	void displayInbox() {
		if (inboxStack.isEmpty()) {
			cout << "End of stack" << endl;
			return;
		}

		cout << left << setw(5) << "ID"
			<< setw(35) << "Sender"
			<< setw(10) << "Priority"
			<< setw(70) << "Subject"
			<< setw(20) << "Date Received"
			<< setw(10) << "Time Received" << endl;
		cout << string(155, '-') << endl;

		int currentRow = 1;
		LinkedListStack<Email> temp;

		while (!inboxStack.isEmpty()) {
			Email currentEmail = inboxStack.getTop();
			cout << left << setw(5) << currentRow
				<< setw(35) << currentEmail.sender
				<< setw(10) << currentEmail.priority
				<< setw(70) << currentEmail.subject
				<< setw(20) << currentEmail.dateReceived
				<< setw(10) << currentEmail.timeReceived << endl;
			currentRow++;
			temp.push(currentEmail);
			inboxStack.pop();
		}

		while (!temp.isEmpty()) {
			Email currentEmail = temp.getTop();
			inboxStack.push(currentEmail);
			temp.pop();
		}
	}

	// Helper function to convert a string to lowercase
	string toLowerCase(const string& str) {
		string lowerStr = str;
		for (char& c : lowerStr) {
			if (c >= 'A' && c <= 'Z') { // Check if character is uppercase
				c = c + ('a' - 'A');     // Convert to lowercase by adding ASCII difference
			}
		}
		return lowerStr;
	}

	// Search for emails by title and display matches
	void searchEmailByTitle(const string& searchString) {
		LinkedListStack<Email> tempStack; // Temporary stack to hold emails
		bool found = false;               // Flag to track if any matching email is found
		int currentRow = 1;                   // Variable to track email index
		string searchLower = toLowerCase(searchString); // Convert search string to lowercase

		// Traverse through the inbox to search for matching titles
		while (!inboxStack.isEmpty()) {
			Email currentEmail = inboxStack.getTop();
			inboxStack.pop();

			// Convert email subject to lowercase for case-insensitive comparison
			string subjectLower = toLowerCase(currentEmail.subject);

			// Check if the email subject contains the search string
			if (subjectLower.find(searchString) != string::npos) {
				if (!found) {
					cout << "\nMatching emails:\n";
					cout << left << setw(5) << "ID"
						<< setw(35) << "Sender"
						<< setw(10) << "Priority"
						<< setw(70) << "Subject"
						<< setw(20) << "Date Received"
						<< setw(10) << "Time Received" << endl;
					cout << string(155, '-') << endl;
					found = true;
				}
				// Display the current email with the index
				cout << left << setw(5) << currentRow
					<< setw(35) << currentEmail.sender
					<< setw(10) << currentEmail.priority
					<< setw(70) << currentEmail.subject
					<< setw(20) << currentEmail.dateReceived
					<< setw(10) << currentEmail.timeReceived << endl;
			}

			tempStack.push(currentEmail); // Push the email to the temporary stack
			currentRow++;                      // Increment index for each email
		}

		if (!found) {
			cout << "No emails found with the title containing \"" << searchString << "\".\n";
		}

		// Restore emails back to the original inbox stack
		while (!tempStack.isEmpty()) {
			inboxStack.push(tempStack.getTop());
			tempStack.pop();
		}
	}

};

void updatePriorityInStack(InboxManagement& emailInbox, Email& targetEmail) {
	LinkedListStack<Email> tempStack;  // Temporary stack to hold emails while updating

	bool updated = false;
	while (!emailInbox.isInboxEmpty()) {
		Email email = emailInbox.viewRecentEmail();
		emailInbox.popRecentEmail();

		// Check if the email matches the target email
		if (email.sender == targetEmail.sender) {  // Assume 'id' uniquely identifies an email
			email.priority = "High";       // Update priority in stack
			updated = true;
		}

		// Push to temporary stack
		tempStack.push(email);
	}

	// Restore the original stack order
	while (!tempStack.isEmpty()) {
		emailInbox.pushEmail(tempStack.getTop());
		tempStack.pop();
	}

	if (updated) {
		cout << "Priority updated in the stack for the email with sender: " << targetEmail.sender << endl;
	}
	else {
		cout << "Email with sender " << targetEmail.sender << " not found in the stack." << endl;
	}
}


#endif // INBOXMANAGEMENTUSINGLLSTACK_HPP
