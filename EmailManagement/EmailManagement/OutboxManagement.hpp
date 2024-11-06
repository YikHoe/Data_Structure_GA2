#include <iostream>
#include <string>
#include <iomanip>  // For formatting output
#include <limits>   // For clearing the input buffer
//#include "SentOutboxManagement.hpp"
using namespace std;

struct OutgoingEmail {
    string receiver;
    string sender;       // Sender of the email
    string priority;     // Priority level of the email (e.g., high, low)
    string subject;      // Subject of the email
    string content;      // Content of the email
    string dateSend;     // Date when the email was received
    string timeSend;     // Time when the email was received
};

struct outgoingQueueNode { // using doubly linked list
    OutgoingEmail email;
    outgoingQueueNode* next, * prev;
};

class OutboxManagement {
    outgoingQueueNode* front, * rear;
    //SentOutboxManagement sentOutbox;
    string QueueName = "";
    int size = 0;

public:
    OutboxManagement(string QueueName) {
        front = rear = nullptr;
        this->QueueName = QueueName;
    }

    ~OutboxManagement() {
        cout << QueueName << " is destroyed now!" << endl;
    }

    outgoingQueueNode* CreateNewNode(OutgoingEmail email) {
        outgoingQueueNode* newNode = new outgoingQueueNode;
        newNode->email = email;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        return newNode;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void enQueue(OutgoingEmail email) { // normal process
        outgoingQueueNode* newNode = CreateNewNode(email);
        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            newNode->prev = rear;
            rear = newNode;
        }
        size++;
    }

    outgoingQueueNode* deQueue(int itemNo) { // remove a specific node
        if (isEmpty() || itemNo <= 0 || itemNo > size) {
            cout << "Invalid item number or Queue is empty." << endl;
            return nullptr;
        }

        outgoingQueueNode* current = front;

        if (itemNo == 1) { // Special case: removing the first node
            return current;
        }

        if (itemNo == size) { // Special case: removing the last node
            current = rear;
            rear = rear->prev;
            rear->next = nullptr;
            size--;
            return current;
        }

        // General case: removing a middle node
        for (int i = 1; i < itemNo; i++) {
            current = current->next;
        }
        current->prev->next = current->next;
        current->next->prev = current->prev;

        size--;
        return current;
    }

    OutgoingEmail dequeue() {
        if (!isEmpty()) {
            OutgoingEmail current = front->email;
            front = front->next;

            if (front != nullptr) {
                front->prev = nullptr;
            }
            else {
                rear = nullptr;
            }

            return current;
        }

        cout << "Queue is empty!" << endl;
        exit(1);
    }

    void enQueueToFront(OutgoingEmail email) { // insert to the front
        outgoingQueueNode* newNode = CreateNewNode(email);
        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            newNode->next = front;
            front->prev = newNode;
            front = newNode;
        }
        size++;
    }

    void displayQueue(int rowNumber = -1) { // display queue content or a specific row with detailed format
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }

        if (rowNumber == -1) {
            // Display table header for all rows
            cout << left << setw(5) << "No"
                << setw(40) << "Sender"
                << setw(10) << "Priority"
                << setw(100) << "Subject"
                << setw(20) << "Date Received"
                << setw(10) << "Time Received" << endl;
            cout << string(200, '-') << endl;
        }

        int currentRow = 1;
        outgoingQueueNode* current = front;
        bool found = false;

        while (current) {
            if (rowNumber == -1 || currentRow == rowNumber) { // Display all rows or specific row
                if (rowNumber == -1) {
                    // Tabular format for all rows
                    cout << left << setw(5) << currentRow
                        << setw(40) << current->email.sender
                        << setw(10) << current->email.priority
                        << setw(100) << current->email.subject
                        << setw(20) << current->email.dateSend
                        << setw(10) << current->email.timeSend << endl;
                }
                else {
                    // Detailed format for a specific row
                    cout << "No: " << rowNumber << endl;
                    cout << "From: " << current->email.sender << endl;
                    cout << "Priority: " << current->email.priority << endl;
                    cout << "Subject: " << current->email.subject << endl;
                    cout << "Content: " << current->email.content << endl;
                    cout << "Date Received: " << current->email.dateSend << endl;
                    cout << "Time Received: " << current->email.timeSend << endl;
                    found = true;
                    break;
                }
            }
            current = current->next;
            currentRow++;
        }

        // If a specific row was requested but not found, display an error
        if (rowNumber != -1 && !found) {
            cout << "Row number " << rowNumber << " not found in the queue." << endl;
        }
    }

    void sentAllDraft() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }

        int currentRow = 1;
        outgoingQueueNode* current = front;

        while (current) {
            // Push email to sent outbox stack
            //sentOutbox.pushToSent(current->email);
            // Remove the email from the queue
            dequeue();

            current = current->next;
            currentRow++;
        }
        cout << "All drafts have been sent and moved to the sent outbox." << endl;
    }

    void displaySentEmails() {
        //sentOutbox.displaySentEmails(); // Call the function from SentOutboxManagement
    }

    void addToDraft(OutgoingEmail email) {
        if (email.priority == "high") {
            // If it's high priority, use enQueueToFront to place it at the front
            enQueueToFront(email);
            cout << "New high-priority email added to the front of the queue!" << endl;
        }
        else {
            // Otherwise, add it normally to the back
            enQueue(email);
            cout << "New email added to the queue." << endl;
        }
    }

    void addNewEmail() {
        OutgoingEmail draft;
        int action;

        cout << "Enter receiver's email: ";
        cin >> draft.receiver;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        cout << "Enter subject: ";
        getline(cin, draft.subject); // First getline

        cout << "Enter content: ";
        getline(cin, draft.content); // Second getline

        cout << "Enter priority: ";
        cin >> draft.priority;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        while (true) {
            cout << "Select the option: 1. Send Email Immediately 2. Save As Draft" << endl;
            cin >> action;

            if (action == 1) {
                // Call send email function
                cout << "Email sent immediately!" << endl;
                break;
            }
            else if (action == 2) {
                addToDraft(draft);
                break;
            }
            else {
                cout << "Please select the action '1' or '2'!" << endl;
            }
        }
    }


};
