#ifndef OUTBOXSTRUCT_HPP
#define OUTBOXSTRUCT_HPP

#include <iostream>
#include <string>
#include <iomanip>  // For formatting output
#include <limits>   // For clearing the input buffer

using namespace std;

struct OutgoingEmail {
    string receiver;
    string sender;
    string priority;
    string subject;
    string content;
    string dateSend;
    string timeSend;
    bool isReply;
    string replySubject;
};

#endif // OUTBOXSTRUCT
