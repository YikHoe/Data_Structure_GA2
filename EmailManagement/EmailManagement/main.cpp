#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "InboxManagementUsingLLStack.hpp"
#include "spamDetector.hpp"
#include "LinkedListQueue.hpp"

using namespace std;

// Function to read a quoted field from the CSV file
string readQuotedField(ifstream& file) {
	string field;
	char ch;
	file.get(ch); // Read the starting quote

	// Read characters until the closing quote followed by a comma is found
	while (file.get(ch)) {
		if (ch == '"' && file.peek() == ',') {
			file.get(); // Consume the comma after the closing quote
			break;
		}
		field += ch; // Append character to the field
	}
	return field;
}

// Function to load emails from a CSV file into the email inbox
void loadEmailsFromFile(LinkedListQueue& emailQueue, const string& filename) {
	ifstream file(filename);
	string sender, priority, subject, content, dateReceived, timeReceived;

	// Loop until the end of the file
	while (file.good()) {
		// Read fields separated by commas
		getline(file, sender, ',');
		getline(file, priority, ',');

		// Handle subject, allowing for quoted fields
		if (file.peek() == '"') subject = readQuotedField(file);
		else getline(file, subject, ',');

		// Handle content, allowing for quoted fields
		if (file.peek() == '"') content = readQuotedField(file);
		else getline(file, content, ',');

		// Read remaining fields
		getline(file, dateReceived, ',');
		getline(file, timeReceived, '\n');

		// Skip header line or empty sender
		if (sender.find("sender") != string::npos) continue;
		if (sender.empty()) break;

        // Create a new Email object and push it onto the inbox
        Email newEmail = { sender, priority, subject, content, dateReceived, timeReceived, false };
        emailQueue.enQueue(newEmail);
    }
}

// load spam words and pharases
void loadSpamWords(spamDetector& detector, const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening spam words file." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t delimPos = line.find(':');
        if (delimPos != string::npos) {
            string word = line.substr(0, delimPos);
            int susWeight = stoi(line.substr(delimPos + 1));

            detector.insert(word, susWeight);
        }
    }

    file.close();
}

// Function to display the inbox in a formatted table
void displayInbox(InboxManagement& emailInbox) {
	LinkedListStack<Email> tempStack; // Temporary stack to hold emails
	int rowNumber = 1;

	// Print header for the email display
	cout << left << setw(5) << "No"
		<< setw(40) << "Sender"
		<< setw(10) << "Priority"
		<< setw(100) << "Subject"
		<< setw(20) << "Date Received"
		<< setw(10) << "Time Received" << endl;
	cout << string(200, '-') << endl;

	// Loop through the inbox to display emails
	while (!emailInbox.isInboxEmpty()) {
		Email email = emailInbox.viewRecentEmail(); // Get the most recent email
		emailInbox.popRecentEmail(); // Remove it from the inbox

		// Print email details in a formatted row
		cout << left << setw(5) << rowNumber++
			<< setw(40) << email.sender
			<< setw(10) << email.priority
			<< setw(100) << email.subject
			<< setw(20) << email.dateReceived
			<< setw(10) << email.timeReceived << endl;

		tempStack.push(email); // Push email onto temporary stack
	}

	// Restore the emails back to the inbox stack
	while (!tempStack.isEmpty()) {
		emailInbox.pushEmail(tempStack.getTop());
		tempStack.pop();
	}
}

// Function to select and display a specific email based on user choice
void selectAndDisplayEmail(InboxManagement& emailInbox, int choice) {
	LinkedListStack<Email> tempStack; // Temporary stack to hold emails
	int rowNumber = 1; // Row number for selection

	// Loop through the inbox until the desired email is reached
	while (!emailInbox.isInboxEmpty() && rowNumber <= choice) {
		Email selectedEmail = emailInbox.viewRecentEmail(); // Get the most recent email
		emailInbox.popRecentEmail(); // Remove it from the inbox

		// If the current row matches the user's choice, display the email
		if (rowNumber == choice) {
			cout << "\nDisplaying email #" << choice << ":\n";
			emailInbox.displayEmail(selectedEmail);
		}

		tempStack.push(selectedEmail); // Push email onto temporary stack
		rowNumber++;
	}

	// Restore the emails back to the inbox stack
	while (!tempStack.isEmpty()) {
		emailInbox.pushEmail(tempStack.getTop());
		tempStack.pop();
	}
}

void preprocessEmail(InboxManagement& emailInbox, LinkedListQueue& emailQueue, spamDetector& detector) {
	while (!emailQueue.isEmpty()) {
		Email email = emailQueue.dequeue();
		if (detector.detectSpam(email.content)) {
			email.isSpam = true;
		}
		emailInbox.pushEmail(email);
	}
}

void moveStackToQueue(InboxManagement& emailInbox, LinkedListQueue& emailQueue) {
	LinkedListStack<Email> tempStack; // Temporary stack to hold emails

	// While the inbox isn't empty, pop recent emails and enqueue them to the queue
	while (!emailInbox.isInboxEmpty()) {
		Email recentEmail = emailInbox.viewRecentEmail();  // Pop the most recent email
		emailQueue.enQueue(recentEmail);                  // Enqueue the email to the queue
		emailInbox.popRecentEmail();

		tempStack.push(recentEmail); // Push email onto temporary stack

	}

	while (!tempStack.isEmpty()) {
		emailInbox.pushEmail(tempStack.getTop());
		tempStack.pop();
	}
}


void displayMainMenu() {
	cout << "\n=== Email Management System ===\n";
	cout << "1. Inbox Management\n";
	cout << "2. Outbox Management\n";
	cout << "3. Quit\n";
	cout << "Please select an option: ";
}

void inboxManagement(InboxManagement& emailInbox, LinkedListQueue& emailQueue) {
	int choice;

	while (true) {
		cout << "\n=== Inbox Management ===\n";
		cout << "1. Display Inbox\n";
		cout << "2. Select and Review an Email\n";
		cout << "3. Filter and Sort Emails by Priority (High to Low)\n";
		cout << "4. Mark an Email as Important\n";
		cout << "5. Display Spam Emails\n";
		cout << "6. Return to Main Menu\n";
		cout << "Please select an option: ";
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a valid number.\n";
			continue;
		}

		switch (choice) {
		case 1:
			emailQueue.displayQueue();
			break;

		case 2: {
			int emailChoice;
			emailQueue.displayQueue();
			cout << "\nEnter the row number to view an email: ";
			cin >> emailChoice;

			if (cin.fail() || emailChoice <= 0 || emailChoice > emailInbox.getInboxSize()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid choice. Please try again.\n";
			}
			else {
				emailQueue.displayQueue(emailChoice);
			}
			break;
		}

		case 3:
			cout << "\nFiltering and sorting emails by priority (High to Low)...\n";
			emailQueue.moveBasedOnPriority();
			emailQueue.displayQueue();
			break;

		case 4: {
			emailQueue.displayQueue();
			int row;
			cout << "Enter the row number to move the email to the front: ";
			cin >> row;
			if (cin.fail() || row <= 0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid row number. Please try again.\n";
			}
			else {
				emailQueue.changePriorityAndMoveToFrontByRow(row);
				emailQueue.displayQueue();
			}
			break;
		}

		case 5: {
			moveStackToQueue(emailInbox, emailQueue);
			emailQueue.displaySpamEmail();
			break;
		}

		case 6:
			return;

		default:
			cout << "Invalid option. Please select a number between 1 and6.\n";
		}
	}
}

int main() {
	// Inbox management init
    InboxManagement emailInbox; 
	LinkedListQueue emailQueue("Email Queue");
    spamDetector sDec;

    loadEmailsFromFile(emailQueue, "DummyEmails.csv"); // load all emails into queue for preprosessing
    loadSpamWords(sDec, "spamWords.txt"); // init spam detector
	preprocessEmail(emailInbox, emailQueue, sDec); // process emails

	int choice;

	while (true) {
		displayMainMenu();
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a valid number.\n";
			continue;
		}

		switch (choice) {
		case 1:
			inboxManagement(emailInbox, emailQueue);
			break;

		case 2:
			cout << "\n=== Outbox Management ===\n";
			cout << "This feature is under construction.\n";
			break;

		case 3:
			cout << "Exiting program.\n";
			return 0;

		default:
			cout << "Invalid option. Please select a number between 1 and 3.\n";
		}
	}

}