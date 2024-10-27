#include <iostream>
#include<iomanip>
#include<fstream>
#include<string>

#include "InboxManagementUsingLLStack.hpp"

using namespace std;

string readQuotedField(ifstream& file) {
    string field, temp;
    char ch;

    // Start by consuming the opening quote
    file.get(ch);

    while (file.get(ch)) {
        // Check for end of quoted field
        if (ch == '"') {
            if (file.peek() == ',') {
                file.get(); // Consume the comma after the quote
                break;
            }
        }
        field += ch;
    }
    return field;
}

int main() {
    InboxManagement emailInbox;
    ifstream file("DummyEmails.csv");
    string sender, priority, subject, content, dateReceived, timeReceived;

    // Read emails from file and push them to stack
    while (file.good()) {
        getline(file, sender, ',');
        getline(file, priority, ',');

        // Check if the subject starts with a quote
        if (file.peek() == '"') {
            subject = readQuotedField(file);
        }
        else {
            getline(file, subject, ',');
        }

        // Check if the content starts with a quote
        if (file.peek() == '"') {
            content = readQuotedField(file);
        }
        else {
            getline(file, content, ',');
        }

        getline(file, dateReceived, ',');
        getline(file, timeReceived, '\n'); // End of line for timeReceived

        if (sender.find("sender") != string::npos) {
            continue; // Skip header row if it contains the word "sender"
        }
        else if (sender == "") {
            break; // If reach an empty row, end of reading
        }

        // Create an Email object from the CSV row data
        Email newEmail = { sender, priority, subject, content, dateReceived, timeReceived };

        // Push the email to the stack
        emailInbox.pushEmail(newEmail);
    }
    int choice = -1;

    while (choice != 0) {
        // Temporary stack to hold emails while displaying
        LinkedListStack<Email> tempStack;

        cout << left << setw(5) << "No"
            << setw(40) << "Sender"
            << setw(10) << "Priority"
            << setw(100) << "Subject"
            << setw(20) << "Date Received"
            << setw(10) << "Time Received" << endl;
        cout << string(200, '-') << endl;

        int rowNumber = 1;
        while (!emailInbox.isInboxEmpty()) {
            // Pop email from main stack and display it
            Email email = emailInbox.viewRecentEmail();
            emailInbox.popRecentEmail();

            cout << left << setw(5) << rowNumber++
                << setw(40) << email.sender
                << setw(10) << email.priority
                << setw(100) << email.subject
                << setw(20) << email.dateReceived
                << setw(10) << email.timeReceived << endl;

            // Push email to the temporary stack to preserve order
            tempStack.push(email);
        }

        // Move emails back from temporary stack to the main stack
        while (!tempStack.isEmpty()) {
            emailInbox.pushEmail(tempStack.getTop());
            tempStack.pop();
        }

        // Ask user to select an email
        cout << "\nEnter the row number to view an email, or 0 to quit: ";
        cin >> choice;

        if (choice > 0 && choice < rowNumber) {
            // Retrieve the selected email by popping each email until we reach the chosen row
            rowNumber = 1;
            while (!emailInbox.isInboxEmpty() && rowNumber <= choice) {
                Email selectedEmail = emailInbox.viewRecentEmail();
                emailInbox.popRecentEmail();

                if (rowNumber == choice) {
                    // Display the selected email
                    cout << "\nDisplaying email #" << choice << ":\n";
                    emailInbox.displayEmail(selectedEmail);
                }
                // Push the email to the temporary stack to preserve order
                tempStack.push(selectedEmail);
                rowNumber++;
            }

            // Restore the main stack's original order
            while (!tempStack.isEmpty()) {
                emailInbox.pushEmail(tempStack.getTop());
                tempStack.pop();
            }
        }
        else if (choice != 0) {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    cout << "Exiting program." << endl;
    return 0;
}