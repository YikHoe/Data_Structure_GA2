#ifndef OUTBOXMANAGEMENT_HPP
#define OUTBOXMANAGEMENT_HPP

#include <iostream>
#include <string>
#include <iomanip>  // For formatting output
#include <limits>   // For clearing the input buffer
#include "OutboxStruct.hpp"

using namespace std;

struct outgoingQueueNode {
    OutgoingEmail email;
    outgoingQueueNode* next;
    outgoingQueueNode* prev;
};

class OutboxManagement {
private:
    outgoingQueueNode* front;
    outgoingQueueNode* rear;
    string QueueName;
    int size;

    outgoingQueueNode* CreateNewNode(OutgoingEmail email);

public:
    OutboxManagement(string QueueName);
    ~OutboxManagement();

    bool isEmpty();
    bool isSentOutboxEmpty();
    void enQueue(OutgoingEmail email);
    outgoingQueueNode* deQueue(int itemNo);
    OutgoingEmail dequeue();
    void enQueueToFront(OutgoingEmail email);
    void displayQueue(int rowNumber = -1);
    void sentAllDraft();
    void displaySentEmails();
    void displayEmailDetails(int rowNumber);
    void addToDraft(OutgoingEmail email);
    void addNewEmail(string receiver = "", string subject = "");
};

#endif // OUTBOXMANAGEMENT_HPP
