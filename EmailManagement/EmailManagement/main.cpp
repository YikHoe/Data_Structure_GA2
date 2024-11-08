#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "InboxManagementUsingLLStack.hpp"
#include "OutboxManagement.hpp"
#include "SentOutboxManagement.hpp"
#include "spamDetector.hpp"


#include "LinkedListQueue.hpp"

using namespace std;

// Function to read a quoted field from the CSV file
string readQuotedField(ifstream& file) {
	string field;
	char ch;
	file.get(ch); // Read the starting quote

	// Read characters until the closing quote followed by a comma iss found
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
        Email newEmail = { sender, priority, subject, content, dateReceived, timeReceived, "", false};
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

            detector.push(word, susWeight);
        }
    }

    file.close();
}

// Function to select and display a specific email based on user choice
void selectAndDisplayEmail(InboxManagement& emailInbox, int choice) {
	LinkedListStack<Email> tempStack; // Temporary stack to hold emails
	int inboxSize = emailInbox.getInboxSize(); // Get the total number of emails
	int rowNumber = 1; // Row number for selection

	// Clear the input buffer and exit if input is invalid
	if (cin.fail() || choice < 1 || choice > inboxSize) {
		cin.clear(); // Clear the error flag
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
		cout << "Invalid input.\n";
		return;
	}

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

bool detectSpam(const string& text, spamDetector& detector) {
	int spamScore = 0;

	// Temporary stack to store elements while checking spam words
	spamDetector tempStack;

	// Loop through the stack, detecting spam words and preserving the stack state
	while (!detector.isEmpty()) {
		spamWords* current = detector.getTop();
		if (text.find(current->word) != string::npos) {
			spamScore += current->susWeight;
		}
		// Move the word to the temp stack
		tempStack.push(current->word, current->susWeight);
		detector.pop();
	}

	// Restore original stack from tempStack
	while (!tempStack.isEmpty()) {
		spamWords* top = tempStack.getTop();
		detector.push(top->word, top->susWeight);
		tempStack.pop();
	}

	if (spamScore > 10) {
		return true;
	}
	else {
		return false;
	}
}

void replySpecifyEmail(InboxManagement& emailInbox, int choice, OutboxManagement& emailOutbox) {
	LinkedListStack<Email> tempStack; // Temporary stack to hold emails
	int rowNumber = 1; // Row number for selection

	// Loop through the inbox until the desired email is reached
	while (!emailInbox.isInboxEmpty() && rowNumber <= choice) {
		Email selectedEmail = emailInbox.viewRecentEmail(); // Get the most recent email
		emailInbox.popRecentEmail(); // Remove it from the inbox

		// If the current row matches the user's choice, display the email
		if (rowNumber == choice) {
			//Pass the email sender, and subject into create new email function
			emailOutbox.addNewEmail(selectedEmail.sender, selectedEmail.subject);
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

void preprocessEmail(InboxManagement& emailInbox, LinkedListQueue& emailQueue, InboxManagement& spamEmailInbox, spamDetector& detector) {
	while (!emailQueue.isEmpty()) {
		Email email = emailQueue.dequeue();
		if (detectSpam(email.content, detector)) {
			email.isSpam = true;
			spamEmailInbox.pushEmail(email);
		}
		else {
			emailInbox.pushEmail(email);
		}
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

void unmarkSpam(InboxManagement& emailInbox, InboxManagement& spamEmailInbox, int emailNum) {
	LinkedListStack<Email> tempStack;
	int rowNumber = 1;

	// Loop through the inbox until the desired email is reached
	while (!spamEmailInbox.isInboxEmpty() && rowNumber <= emailNum) {
		Email selectedEmail = spamEmailInbox.viewRecentEmail();
		spamEmailInbox.popRecentEmail();

		// If the current row matches the user's choice, display the email
		if (rowNumber == emailNum) {
			selectedEmail.isSpam = false;
			emailInbox.pushEmail(selectedEmail);
			break;
		}

		tempStack.push(selectedEmail); // Push email onto temporary stack
		rowNumber++;
	}

	// Restore the emails back to the inbox stack
	while (!tempStack.isEmpty()) {
		spamEmailInbox.pushEmail(tempStack.getTop());
		tempStack.pop();
	}
}

void inboxManagement(InboxManagement& emailInbox, LinkedListQueue& emailQueue, InboxManagement& spamEmailInbox) {
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
			emailInbox.displayInbox();
			break;

		case 2: {
			int emailChoice;
			emailInbox.displayInbox();
			cout << "\nEnter the email ID to view: ";
			cin >> emailChoice;

			selectAndDisplayEmail(emailInbox, emailChoice);
			break;
		}

		case 3:
			cout << "\nFiltering and sorting emails by priority (High to Low)...\n";
			moveStackToQueue(emailInbox, emailQueue);
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
			spamEmailInbox.displayInbox();
			int row;
			cout << "Enter the row number to view the email [0 to exit]: ";
			cin >> row;
			if (cin.fail() || row < 0 || row > spamEmailInbox.getInboxSize()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid row number. Please try again.\n";
			}
			else {
				selectAndDisplayEmail(spamEmailInbox, row);
				int choice;
				cout << "\n=== Spam Management ===\n";
				cout << "1. Mark as Not Spam\n";
				cout << "2. Exit\n";
				cin >> choice;
				if (cin.fail() || choice <= 0 || choice > 2) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Invalid input.\n";
				}
				else if (choice == 1) {
					unmarkSpam(emailInbox, spamEmailInbox, row);
					cout << "Email " << row << " is removed from spam." << endl;
				}

			}
			break;
		}

		case 6:
			return;

		default:
			cout << "Invalid option.\n";
		}
	}
}


void outboxManagement(InboxManagement& emailInbox, OutboxManagement& emailOutbox, SentOutboxManagement& emailSent) {
	int choice;

	while (true) {
		cout << "\n=== Outbox Management ===\n";
		cout << "1. Display Outbox\n";
		cout << "2. View Specify Outbox\n";
		cout << "3. Display Draft\n";
		cout << "4. Reply an Email\n";
		cout << "5. Create New Emails\n";
		cout << "6. Sent All Draft Emails\n";
		cout << "7. Return to Main Menu\n";
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
			//Call display sent email function here
			emailOutbox.displaySentEmails();
			break;

		case 2:
			if (!emailOutbox.isSentOutboxEmpty()) {
				while (true) {
					int emailChoice;
					emailOutbox.displaySentEmails();
					cout << "\Select the row number to view outbox detail: ";
					cin >> emailChoice;
					if (cin.fail() || emailChoice <= 0 || emailChoice > emailInbox.getInboxSize()) {
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Invalid choice. Please try again.\n";
					}
					else {
						emailOutbox.displayEmailDetails(emailChoice);
						break;
					}
				}
			}
			else {
				cout << "The outbox is empty!" << endl;
			}
			break;

		case 3:
			//Display draft using LLQueue
			emailOutbox.displayQueue();
			break;

		case 4: 
			while (true) {
				int replyChoice;
				emailInbox.displayInbox();
				cout << "\nEnter the row number to reply an email: ";
				cin >> replyChoice;


				if (cin.fail() || replyChoice <= 0 || replyChoice > emailInbox.getInboxSize()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Invalid choice. Please try again.\n";
				}
				else {
					replySpecifyEmail(emailInbox, replyChoice, emailOutbox);
					break;
				}
			}
			break;

		case 5:
			cout << "\nCreating New Email... \n";
			emailOutbox.addNewEmail();
			break;

		case 6:
			cout << "\Sending All Email... \n";
			emailOutbox.sentAllDraft();
			break;

		case 7:
			//Check if got email to upload, if yes ask to upload
			if (!emailOutbox.isEmpty()) {
				while (true) {
					string sentEmail;
					cout << "There are email draft, do you want to send it? (y/n)" << endl;
					cin >> sentEmail;
					if (sentEmail == "y") {
						emailOutbox.sentAllDraft();
						break;
					}
					else if (sentEmail == "n") {
						break;
					}
					else {
						cout << "Please select 'y' for yes or 'n' for no." << endl;
					}
				}
			}
			return;
			

		default:
			cout << "Invalid option. Please select a number between 1 and6.\n";
			break;
		}
	}
}

int main() {
	// Inbox management init
    InboxManagement emailInbox("Email Stack");
	InboxManagement spamEmails("Spam Stack");
	LinkedListQueue emailQueue("Email Queue");
	OutboxManagement emailOutbox("Email Outbox");
	SentOutboxManagement emailSent;
    spamDetector sDec;

    loadEmailsFromFile(emailQueue, "DummyEmails.csv"); // load all emails into queue for preprosessing
    loadSpamWords(sDec, "spamWords.txt"); // init spam detector
	preprocessEmail(emailInbox, emailQueue, spamEmails, sDec); // process emails

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
			inboxManagement(emailInbox, emailQueue, spamEmails);
			break;

		case 2:
			outboxManagement(emailInbox, emailOutbox, emailSent);
			break;

		case 3:
			cout << "Exiting program.\n";
			return 0;

		default:
			cout << "Invalid option. Please select a number between 1 and 3.\n";
		}
	}

}