#include "SentOutboxManagement.hpp"
// Constructor
SentOutboxManagement::SentOutboxManagement() : topNode(nullptr), stackSize(0) {}

// Destructor
SentOutboxManagement::~SentOutboxManagement() {
    cout << "Sent outbox is destroyed now!" << endl;
}

// Create a new node with the given email
OutgoingStackNode* SentOutboxManagement::createNewNode(const OutgoingEmail& email) {
    OutgoingStackNode* newNode = new OutgoingStackNode;
    newNode->email = email;
    newNode->nextAdd = nullptr;
    return newNode;
}

// Push an email to the sent stack
void SentOutboxManagement::pushToSent(const OutgoingEmail& email) {
    OutgoingStackNode* newNode = createNewNode(email);
    if (stackSize == 0) {
        topNode = newNode;
    }
    else {
        newNode->nextAdd = topNode;
        topNode = newNode;
    }
    stackSize++;
    cout << "Email sent and added to sent stack!" << endl;
}

// Check if the sent stack is empty
bool SentOutboxManagement::isEmpty() const {
    return topNode == nullptr;
}

// Display all sent emails
void SentOutboxManagement::displaySentEmails() {
    if (isEmpty()) {
        cout << "Sent outbox is empty." << endl;
        return;
    }

    cout << left << setw(5) << "No"
        << setw(40) << "Sender"
        << setw(10) << "Priority"
        << setw(100) << "Subject"
        << setw(20) << "Date Sent"
        << setw(10) << "Time Sent" << endl;
    cout << string(200, '-') << endl;

    OutgoingStackNode* currentNode = topNode;
    int currentRow = 1;

    while (currentNode != nullptr) {
        const OutgoingEmail& email = currentNode->email;
        cout << left << setw(5) << currentRow
            << setw(40) << email.sender
            << setw(10) << email.priority
            << setw(100) << email.subject
            << setw(20) << email.dateSend
            << setw(10) << email.timeSend << endl;
        currentNode = currentNode->nextAdd;
        currentRow++;
    }
}

// Display details of a specific sent email
void SentOutboxManagement::displayDetailedSentEmail(int emailNo) {
    if (isEmpty()) {
        cout << "Sent outbox is empty." << endl;
        return;
    }

    OutgoingStackNode* currentNode = topNode;
    int currentRow = 1;

    while (currentNode != nullptr) {
        const OutgoingEmail& email = currentNode->email;

        if (currentRow == emailNo) {
            cout << "No: " << currentRow << endl;
            cout << "From: " << email.sender << endl;
            cout << "Priority: " << email.priority << endl;
            cout << "Subject: " << email.subject << endl;
            cout << "Content: " << email.content << endl;
            cout << "Date Sent: " << email.dateSend << endl;
            cout << "Time Sent: " << email.timeSend << endl;
            return;
        }

        currentNode = currentNode->nextAdd;
        currentRow++;
    }

    cout << "Email number " << emailNo << " not found in the sent outbox." << endl;
}
