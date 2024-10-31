#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "InboxManagementUsingLLStack.hpp"

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
void loadEmailsFromFile(InboxManagement& emailInbox, const string& filename) {
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
        emailInbox.pushEmail(newEmail);
    }
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

// Main function to run the email inbox management program
int main() {
    InboxManagement emailInbox; 
    loadEmailsFromFile(emailInbox, "DummyEmails.csv"); 

    int choice; 
    while (true) {
        displayInbox(emailInbox);

        cout << "\nEnter the row number to view an email, or 0 to quit: ";
        cin >> choice; 

        // Check for invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number.\n";
            continue;
        }

        // Exit if user chooses 0
        if (choice == 0) break;
        // Display the selected email if the choice is valid
        else if (choice > 0 && choice <= emailInbox.getInboxSize()) {
            selectAndDisplayEmail(emailInbox, choice);
        }
        else {
            cout << "Invalid choice. Please try again." << endl; 
        }
    }

    cout << "Exiting program." << endl; 
    return 0;
}
