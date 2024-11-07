#ifndef SENTOUTBOXMANAGEMENT_HPP
#define SENTOUTBOXMANAGEMENT_HPP

#include <iostream>
#include <string>
#include <iomanip>  // For formatting output
#include "OutboxStruct.hpp"

using namespace std;

struct OutgoingStackNode {
    OutgoingEmail email;
    OutgoingStackNode* nextAdd;
};

class SentOutboxManagement {
private:
    OutgoingStackNode* topNode;
    int stackSize;

public:
    SentOutboxManagement();
    ~SentOutboxManagement();

    OutgoingStackNode* createNewNode(const OutgoingEmail& email);
    void pushToSent(const OutgoingEmail& email);
    bool isEmpty() const;
    void displaySentEmails();
    void displayDetailedSentEmail(int emailNo);
};

#endif // SENTOUTBOXMANAGEMENT_HPP
