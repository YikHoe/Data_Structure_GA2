#ifndef SENTOUTBOXMANAGEMENT_HPP
#define SENTOUTBOXMANAGEMENT_HPP

#include <iostream>
#include <string>
#include <iomanip>  // For formatting output
#include "OutboxManagement.hpp"  // Include the header to use OutgoingEmail

using namespace std;

class SentOutboxManagement {
private:
    struct OutgoingStackNode {  // Renamed the node to OutgoingStackNode
        OutgoingEmail email;   // Email to store in the node
        OutgoingStackNode* nextAdd;  // Pointer to the next node in the stack
    };

    OutgoingStackNode* topNode;  // Pointer to the top node
    int stackSize;  // To keep track of the size of the stack

public:
    SentOutboxManagement() : topNode(nullptr), stackSize(0) {}

    ~SentOutboxManagement() {
        cout << "Sent outbox is destroyed now!" << endl;
    }

    // Create a new node with the given email
    OutgoingStackNode* createNewNode(const OutgoingEmail& email) {
        OutgoingStackNode* newNode = new OutgoingStackNode;
        newNode->email = email;
        newNode->nextAdd = nullptr;
        return newNode;
    }

    // Push an email to the sent stack (manual implementation)
    void pushToSent(const OutgoingEmail& email) {
        OutgoingStackNode* newNode = createNewNode(email);

        // If the stack is empty, set both topNode and bottomNode to new node
        if (stackSize == 0) {
            topNode = newNode;
        }
        else {
            newNode->nextAdd = topNode;
            topNode = newNode;  // Update topNode to point to the new node
        }

        stackSize++;  // Increase stack size
        cout << "Email sent and added to sent stack!" << endl;
    }

    // Check if the sent stack is empty
    bool isEmpty() const {
        return topNode == nullptr;
    }

    // Display all sent emails
    void displaySentEmails() {
        if (isEmpty()) {
            cout << "Sent outbox is empty." << endl;
            return;
        }

        // Display table header
        cout << left << setw(5) << "No"
            << setw(40) << "Sender"
            << setw(10) << "Priority"
            << setw(100) << "Subject"
            << setw(20) << "Date Sent"
            << setw(10) << "Time Sent" << endl;
        cout << string(200, '-') << endl;

        OutgoingStackNode* currentNode = topNode;  // Start from the top node
        int currentRow = 1;

        // Traverse through the stack and display email details
        while (currentNode != nullptr) {
            const OutgoingEmail& email = currentNode->email;
            cout << left << setw(5) << currentRow
                << setw(40) << email.sender
                << setw(10) << email.priority
                << setw(100) << email.subject
                << setw(20) << email.dateSend
                << setw(10) << email.timeSend << endl;
            currentNode = currentNode->nextAdd;  // Move to the next node
            currentRow++;
        }
    }

    // Display details of a specific sent email
    void displayDetailedSentEmail(int emailNo) {
        if (isEmpty()) {
            cout << "Sent outbox is empty." << endl;
            return;
        }

        OutgoingStackNode* currentNode = topNode;  // Start from the top node
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

            currentNode = currentNode->nextAdd;  // Move to the next node
            currentRow++;
        }

        cout << "Email number " << emailNo << " not found in the sent outbox." << endl;
    }

    // Clear all sent emails from the stack
    //void clearSentOutbox() {
    //    while (topNode != nullptr) {
    //        OutgoingStackNode* tempNode = topNode;
    //        topNode = topNode->nextAdd;  // Move the top node pointer
    //        delete tempNode;  // Delete the old top node
    //    }
    //    stackSize = 0;
    //    cout << "All sent emails have been cleared from the sent outbox." << endl;
    //}
};

#endif // SENTOUTBOXMANAGEMENT_HPP
