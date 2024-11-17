#include "SentOutboxManagement.hpp"
#include <ctime>  // For time functions
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
void SentOutboxManagement::pushToSent(OutgoingEmail& email) {
    // Get the current time
    time_t now = time(0);
    tm localTime;
    
    // Use localtime_s for thread safety
    localtime_s(&localTime, &now);

    // Format date as "YYYY-MM-DD"
    char dateBuffer[11];  // Enough for "YYYY-MM-DD" + null terminator
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &localTime);
    //set the send date
    email.dateSend = dateBuffer;

    // Format time as "HH:MM"
    char timeBuffer[6];  // Enough for "HH:MM" + null terminator
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", &localTime);
    //set the send time
    email.timeSend = timeBuffer;


    // Create a new node 
    OutgoingStackNode* newNode = createNewNode(email);
    //Push email to stack
    // If is empty, just replace the top node with newnode
    if (stackSize == 0) {
        topNode = newNode;
    } 
    // direct the next address of the new node to current top node
    // then replace the top node with the new node
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
        << setw(40) << "Sent To"
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
            << setw(40) << email.receiver
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
            cout << endl;
            cout << string(50, '=') << "Outbox Details" << string(50, '=') << endl;
            if (email.isReply) {
                cout << "Reply To: " << email.receiver << endl;
                cout << "Reply Subject: " << email.replySubject << endl;
            }
            else {
                cout << "Sent To: " << email.receiver << endl;
            }
            cout << endl;
            cout << string(114, '-') << endl;
            cout << endl;
            cout << "=== Outbox No: " << currentRow << " ===" << endl;
            cout << "Subject: " << email.subject << endl;
            cout << "Content: " << email.content << endl;
            cout << "Date Sent: " << email.dateSend << endl;
            cout << "Time Sent: " << email.timeSend << endl;
            cout << string(114, '=') << endl;
            return;
        }

        currentNode = currentNode->nextAdd;
        currentRow++;
    }

    cout << "Email number " << emailNo << " not found in the sent outbox." << endl;
}
