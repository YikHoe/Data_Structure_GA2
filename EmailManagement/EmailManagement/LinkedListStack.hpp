#pragma once
#ifndef LINKEDLISTSTACK_HPP
#define LINKEDLISTSTACK_HPP

#include <iostream>
#include <stdexcept> // For exception handling
using namespace std;

// Custom Linked List Stack Template Class
template <typename T>
class LinkedListStack {
private:
    // Node structure for the linked list
    struct Node {
        T data;         // Data stored in the node
        Node* nextAdd;     // Pointer to the next node in the stack

        // Node constructor
        Node(const T& data) : data(data), nextAdd(nullptr) {}
    };

    Node* topNode, * bottomNode = nullptr;  // Pointer to the top and bottom node in the stack
    int stackSize = 0;   // Variable to keep track of the stack size

public:
    // Constructor: Initialize stack with no elements
    LinkedListStack() {
        stackSize = 0;
        topNode = bottomNode = nullptr;
    }

    // Destructor: Clean up memory by deleting all nodes
    ~LinkedListStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Function to create a new node
    Node* createNewNode(const T& element){
        Node* newNode = new Node(element);
        newNode->nextAdd = nullptr;
        return newNode;
    }

    // Function to check if the stack is empty
    bool isEmpty() const {
        if (topNode == nullptr){
            return true;
        }
        return false;
    }

    // Function to add (push) an element to the stack
    void push(const T& element) {
        Node* newNode = createNewNode(element);
        if (isEmpty()){
            topNode = bottomNode = newNode;
        }
        else{
            newNode->nextAdd = topNode;
            topNode = newNode;
        }
        stackSize++;
    }

    // Function to remove (pop) the top element of the stack
    void pop() {
        if (isEmpty()) {
            throw underflow_error("Stack is empty. Cannot pop.");
        }
        Node* current = topNode;
        topNode = topNode->nextAdd;
        if (topNode == nullptr){
            bottomNode = nullptr;
        }
        delete current;  // Free memory of the removed node
        stackSize--;
    }

    // Function to return the top element of the stack
    T& getTop() const {
        if (isEmpty()) {
            throw underflow_error("Stack is empty. No top element.");
        }
        return topNode->data;
    }

    // Function to return the current size of the stack
    int getSize() const {
        return stackSize;
    }

    // Function to display the stack elements
    void displayStack() const {
        Node* current = topNode;  // Start from the top node
        while (current != nullptr) {
            cout << current->data << endl;  // Print the data of the current node
            current = current->nextAdd;     // Move to the next node
        }
        cout << "End of stack." << endl; // Indicate the end of the stack
    }
};

#endif // LINKEDLISTSTACK_HPP