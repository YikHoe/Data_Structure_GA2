#include "SentOutboxManagement.hpp"
#include "OutboxManagement.hpp"
SentOutboxManagement sentOutbox;

OutboxManagement::OutboxManagement(string QueueName) {
    front = rear = nullptr;
    this->QueueName = QueueName;
    size = 0;
}

OutboxManagement::~OutboxManagement() {
    cout << QueueName << " is destroyed now!" << endl;
}

outgoingQueueNode* OutboxManagement::CreateNewNode(OutgoingEmail email) {
    outgoingQueueNode* newNode = new outgoingQueueNode;
    newNode->email = email;
    newNode->next = nullptr;
    newNode->prev = nullptr;
    return newNode;
}

bool OutboxManagement::isEmpty() {
    return front == nullptr;
}

void OutboxManagement::enQueue(OutgoingEmail email) {
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

outgoingQueueNode* OutboxManagement::deQueue(int itemNo) {
    if (isEmpty() || itemNo <= 0 || itemNo > size) {
        cout << "Invalid item number or Queue is empty." << endl;
        return nullptr;
    }

    outgoingQueueNode* current = front;

    if (itemNo == 1) {
        return current;
    }

    if (itemNo == size) {
        current = rear;
        rear = rear->prev;
        rear->next = nullptr;
        size--;
        return current;
    }

    for (int i = 1; i < itemNo; i++) {
        current = current->next;
    }
    current->prev->next = current->next;
    current->next->prev = current->prev;

    size--;
    return current;
}

OutgoingEmail OutboxManagement::dequeue() {
    if (!isEmpty()) {
        OutgoingEmail current = front->email;
        front = front->next;

        if (front != nullptr) {
            front->prev = nullptr;
        }
        else {
            rear = nullptr;
        }
        cout << "An email draft has been dequeue" << endl;
        return current;
    }

    cout << "Queue is empty!" << endl;
    exit(1);
}

void OutboxManagement::enQueueToFront(OutgoingEmail email) {
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

void OutboxManagement::displayQueue(int rowNumber) {
    if (isEmpty()) {
        cout << "Queue is empty." << endl;
        return;
    }

    if (rowNumber == -1) {
        // Display table header
        cout << left << setw(5) << "No"
            << setw(40) << "Sent To"
            << setw(10) << "Priority"
            << setw(100) << "Subject" << endl;
        cout << string(155, '-') << endl;  // Adjust the length to match the width of the header
    }

    int currentRow = 1;
    outgoingQueueNode* current = front;
    bool found = false;

    while (current) {
        if (rowNumber == -1 || currentRow == rowNumber) {
            if (rowNumber == -1) {
                // Display rows for all emails
                cout << left << setw(5) << currentRow
                    << setw(40) << current->email.receiver
                    << setw(10) << current->email.priority
                    << setw(100) << current->email.subject << endl;
            }
            else {
                // Display detailed information for a specific row
                cout << "No: " << rowNumber << endl;
                cout << "Sent To: " << current->email.receiver << endl;
                cout << "Priority: " << current->email.priority << endl;
                cout << "Subject: " << current->email.subject << endl;
                found = true;
                break;
            }
        }
        current = current->next;
        currentRow++;
    }

    if (rowNumber != -1 && !found) {
        cout << "Row number " << rowNumber << " not found in the queue." << endl;
    }
}



void OutboxManagement::sentAllDraft() {
    if (isEmpty()) {
        cout << "Queue is empty." << endl;
        return;
    }

    int currentRow = 1;
    outgoingQueueNode* current = front;

    while (current) {
        //Sent Email to Stack
        sentOutbox.pushToSent(current->email);
        dequeue();
        current = current->next;
        currentRow++;
    }
    cout << "All drafts have been sent and moved to the sent outbox." << endl;
}


void OutboxManagement::displaySentEmails() {
     sentOutbox.displaySentEmails(); // Call the function from SentOutboxManagement
}

bool OutboxManagement::isSentOutboxEmpty() {
    return sentOutbox.isEmpty(); // Call the function from SentOutboxManagement
}

void OutboxManagement::displayEmailDetails(int rowNumber) {
    sentOutbox.displayDetailedSentEmail(rowNumber); // Call the function from SentOutboxManagement
}

void OutboxManagement::addToDraft(OutgoingEmail email) {
    if (email.priority == "high") {
        enQueueToFront(email);
        cout << "New high-priority email added to the front of the queue!" << endl;
    }
    else {
        enQueue(email);
        cout << "New email added to the queue." << endl;
    }
}

void OutboxManagement::addNewEmail(string receiver, string subject) {
    OutgoingEmail draft;
    int action;
    int priorityChoice;
    if (receiver == "") {
        // If the user create new email
        cout << "Enter receiver's email: ";
        cin >> draft.receiver;
        //Set email draft as non replying
        draft.isReply = false;
    }
    else {
        // If the user reply to any email
        cout << "Replying to: " << receiver << endl;
        //Set email draft as non replying
        draft.isReply = true;
        //Set the receiver automatically
        draft.receiver = receiver;
        //Set the subject
        draft.replySubject = subject;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter subject: ";
    getline(cin, draft.subject);

    cout << "Enter content: ";
    getline(cin, draft.content);
    while (true) {
        cout << "Email priority:" << endl;
        cout << "1. High priority (email would queue on top)" << endl;
        cout << "2. Normal priority" << endl;
        cout << "Select the email priority (1 or 2): ";
        cin >> priorityChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (priorityChoice == 1) {
            draft.priority = "high";
            break;
        }
        else if (priorityChoice == 2) {
            draft.priority = "normal";
            break;
        }
           
        else
            cout << "Please select the option either 1 or 2!" << endl;
    }

    while (true) {
        cout << "Email priority:" << endl;
        cout << "1. Send Email Immediately" << endl;
        cout << "2. Save Email As Draft" << endl;
        cout << "Select the option (1 or 2): " ;
        cin >> action;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please try again.\n";
        }

        if (action == 1) {
            sentOutbox.pushToSent(draft);
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

