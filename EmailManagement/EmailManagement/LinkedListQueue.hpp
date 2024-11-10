#include <iostream>
#include <string>
#include "LinkedListStack.hpp"
using namespace std;

struct queueNode // using doubly linked list
{
	Email email;
	queueNode* next, * prev;
};

class LinkedListQueue
{
	queueNode* front, * rear;
	string QueueName = "";
	int size = 0;

public:
	LinkedListQueue(string QueueName)
	{
		front = rear = NULL;
		this->QueueName = QueueName;
	}
	~LinkedListQueue()
	{
		cout << QueueName << " is destroyed now!" << endl;
	}

	queueNode* CreateNewNode(Email email)
	{
		queueNode* newNode = new queueNode;
		newNode->email = email;
		newNode->next = nullptr;
		newNode->prev = nullptr;
		return newNode;
	}

	bool isEmpty()
	{
		return front == nullptr;
	}

	void enQueue(Email email) // normal process
	{
		queueNode* newNode = CreateNewNode(email);
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

	queueNode* deQueue(int itemNo) // remove a specific node
	{
		if (isEmpty() || itemNo <= 0 || itemNo > size) {
			cout << "Invalid item number or Queue is empty." << endl;
			return nullptr;
		}

		queueNode* current = front;

		if (itemNo == 1) { // Special case: removing the first node
			return current;
		}

		if (itemNo == size) { // Special case: removing the last node
			current = rear;
			rear = rear->prev->next;
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

	Email dequeue() {
		if (!isEmpty()) {
			Email current = front->email;
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

	void enQueueToFront(Email email) // insert to the front
	{
		queueNode* newNode = CreateNewNode(email);
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

	void displayQueue(int rowNumber = -1) // display queue content or a specific row with detailed format
	{
		if (isEmpty()) {
			cout << "Queue is empty." << endl;
			return;
		}

		if (rowNumber == -1) {
			// Display table header for all rows
			cout << left << setw(5) << "No"
				<< setw(35) << "Sender"
				<< setw(10) << "Priority"
				<< setw(70) << "Subject"
				<< setw(20) << "Date Received"
				<< setw(10) << "Time Received" << endl;
			cout << string(155, '-') << endl;
		}

		int currentRow = 1;
		queueNode* current = front;
		bool found = false;

		while (current) {
			if (rowNumber == -1 || currentRow == rowNumber) { // Display all rows or specific row
				if (rowNumber == -1) {
					// Tabular format for all rows
					cout << left << setw(5) << currentRow
						<< setw(35) << current->email.sender
						<< setw(10) << current->email.priority
						<< setw(70) << current->email.subject
						<< setw(20) << current->email.dateReceived
						<< setw(10) << current->email.timeReceived << endl;
				}
				else {
					// Detailed format for a specific row
					cout << "No: " << rowNumber << endl;
					cout << "From: " << current->email.sender << endl;
					cout << "Priority: " << current->email.priority << endl;
					cout << "Subject: " << current->email.subject << endl;
					cout << "Content: " << current->email.content << endl;
					cout << "Date Received: " << current->email.dateReceived << endl;
					cout << "Time Received: " << current->email.timeReceived << endl;
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

	void moveBasedOnPriority() {
		if (isEmpty()) {
			cout << "Queue is empty." << endl;
			return;
		}

		queueNode* current = front;
		queueNode* lastHigh = nullptr;  // Track the last moved high-priority node

		while (current != nullptr) {
			// Check if the current node is a high-priority email
			if (current->email.priority == "High") {
				// If it’s already at the front, move on to the next
				if (current == front) {
					lastHigh = current;  // Update lastHigh to this node
					current = current->next;
					continue;
				}

				// If it's not at the front, we move it just after the last high-priority node
				queueNode* nodeToMove = current;
				current = current->next;  // Move to next node before re-linking

				// Detach the node to move
				if (nodeToMove->prev) nodeToMove->prev->next = nodeToMove->next;
				if (nodeToMove->next) nodeToMove->next->prev = nodeToMove->prev;

				// Insert the node after the last high-priority node
				if (lastHigh == nullptr) {  // First high-priority node becomes the new front
					nodeToMove->next = front;
					front->prev = nodeToMove;
					front = nodeToMove;
					nodeToMove->prev = nullptr;
				}
				else {
					nodeToMove->next = lastHigh->next;
					if (lastHigh->next) lastHigh->next->prev = nodeToMove;
					lastHigh->next = nodeToMove;
					nodeToMove->prev = lastHigh;
				}

				// Update the lastHigh pointer to the newly moved node
				lastHigh = nodeToMove;
			}
			else {
				// Move to the next node if it's not high priority
				current = current->next;
			}
		}

		cout << "All high-priority emails have been moved to the front." << endl;
	}

	void changePriorityAndMoveToFrontByRow(int rowNumber, InboxManagement& emailInbox) {
		if (isEmpty()) {
			cout << "Queue is empty." << endl;
			return;
		}

		// Start from the front of the queue
		queueNode* current = front;
		int currentRow = 1; // Keep track of the row number

		// Traverse to the specified row
		while (current != nullptr && currentRow < rowNumber) {
			current = current->next;
			currentRow++;
		}

		// Check if the row exists in the queue
		if (current == nullptr) {
			cout << endl << "Row " << rowNumber << " not found in the queue." << endl;
			return;
		}

		// Check if the priority is already "High"
		if (current->email.priority == "High") {
			cout << endl << "Email at row " << rowNumber << " already has 'High' priority. Only Moving to the front." << endl;
		}
		else {
			// Change the priority to "High" in the queue
			current->email.priority = "High";
			cout << endl << "Priority of email at row " << rowNumber << " changed to 'High' and moved to the front." << endl;

			// Call the stack update function to update priority in the stack
			updatePriorityInStack(emailInbox, current->email);
		}

		// If the node is already at the front, no need to move
		if (current == front) {
			cout << endl << "Email at row " << rowNumber << " is already at the front." << endl;
			return;
		}

		// Detach the node from its current position
		if (current->prev) current->prev->next = current->next;
		if (current->next) current->next->prev = current->prev;

		// Move the node to the front of the queue
		current->next = front;
		front->prev = current;
		front = current;
		front->prev = nullptr;
	}


	void changePriorityAndMoveToBackByRow(int rowNumber, InboxManagement& emailInbox) {
		if (isEmpty()) {
			cout << "Queue is empty." << endl;
			return;
		}

		// Start from the front of the queue
		queueNode* current = front;
		int currentRow = 1; // Keep track of the row number

		// Traverse to the specified row
		while (current != nullptr && currentRow < rowNumber) {
			current = current->next;
			currentRow++;
		}

		// Check if the row exists in the queue
		if (current == nullptr) {
			cout << endl << "Row " << rowNumber << " not found in the queue." << endl;
			return;
		}

		// Check if the priority is already "Low"
		if (current->email.priority == "Low") {
			cout << endl << "Email at row " << rowNumber << " already has 'Low' priority. Only Moving to the back." << endl;
		}
		else {
			// Change the priority to "Low" in the queue
			current->email.priority = "Low";
			cout << endl << "Priority of email at row " << rowNumber << " changed to 'Low' and moved to the back." << endl;

			// Call the stack update function to update priority in the stack
			updatePriorityInStack(emailInbox, current->email);
		}

		// If the node is already at the back, no need to move
		if (current == rear) {
			cout << endl << "Email at row " << rowNumber << " is already at the back." << endl;
			return;
		}

		// Detach the node from its current position
		if (current == front) {
			// If it's the front node, update the front pointer
			front = front->next;
			if (front) front->prev = nullptr;
		}
		else {
			// If it's a middle node, adjust its neighbors
			if (current->prev) current->prev->next = current->next;
			if (current->next) current->next->prev = current->prev;
		}

		// Move the node to the back of the queue
		current->prev = rear;
		current->next = nullptr;

		if (rear) rear->next = current;
		rear = current;  // Update the rear pointer
	}



};