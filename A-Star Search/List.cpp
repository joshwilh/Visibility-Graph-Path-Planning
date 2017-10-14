//
//  List.cpp
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "Tree_Node.h"
#include "List.h"
using namespace std;

// Constructs empty list
List::List() {
    // Set to null to avoid garbage value
    head = nullptr;
    tail = nullptr;
}

// Destructor: Deletes all nodes in list
List::~List() {
    if(!empty()) {
        // Delete all nodes
        List_Node* current = head;
        List_Node* after = head->next;
        
        delete current;
        while (after != nullptr) {
            current = after;
            after = after->next;
            delete current;        
        }
    }
}

// Adds a node to the end of the list
void List::insertEnd(Tree_Node* nodePtr) {
    List_Node *temp = new List_Node;
    temp->data = nodePtr;
    temp->next = nullptr;
    if(head == nullptr) {
        // Empty linked list -- new node is head and tail
        head = temp;
        tail = temp;
        temp = nullptr;
    }
    else {
        // Not empty linked list
        // New node is new tail, old tail gets address of new tail
        tail->next = temp;
        tail = temp;
    }
}

// Adds a node to the beginning of the list
void List::insertStart(Tree_Node* nodePtr) {
    List_Node *temp = new List_Node;
    temp->data = nodePtr;
    if(head == nullptr) {
        // Empty List
        temp->next = nullptr;
        head = temp;
        tail = temp;
        temp = nullptr;
    }
    else {
        temp->next = head;
        head = temp;
    }
}

// Adds a node to the list based on its f-cost
// Node is inserted such that all nodes are sorted by f-cost
// smallest to largest from head to tail
void List::insertByValue(Tree_Node* nodePtr) {
    // If list empty, insert using insertStart
    if (empty()) {
        insertStart(nodePtr);
        return;
    }
    
    List_Node *temp = new List_Node;
    temp->data = nodePtr;
    
    int newFCost = nodePtr->fCost;
    
    // Check if new node needs to be inserted at start
    if (newFCost < head->data->fCost) {
        insertStart(nodePtr);
        return;
    }
    
    // Traverse list
    List_Node *current = head;
    List_Node *after = head->next;
    while (after != nullptr) {
        if (newFCost < after->data->fCost) {
            // Found node to insert before (after)
            temp->next = after;
            current->next = temp;
            return;
        }
        current = current->next;
        after = after->next;
    }
    
    // No match was found before last node, so new node must go at end
    insertEnd(nodePtr);
    return;
}

// Removes the node at the end of the list and returns its data
// WARNING: SLOW in this implementation
Tree_Node* List::removeEnd() {
    // Check if empty list
    assert(head != nullptr);
    
    if (head->next == nullptr) {
        // Single node
        return deleteSingleNode();
    }
    else {
        // Traverse list, end when "current" is second to last node and "after" is tail
        List_Node *current = head;
        List_Node *after = current->next;
        while (after->next != nullptr) {
            current = after;
            after = current->next;
        }
        current->next = nullptr;
        tail = current;
        Tree_Node* returnData = after->data;
        delete after;
        return returnData;
    }
}

// Removes the node at the beginning of the list and returns its data
Tree_Node* List::removeStart() {
    // Check if empty list
    assert(head != nullptr);
    
    if (head->next == nullptr) {
        // Single node
        return deleteSingleNode();
    }
    List_Node *temp = head;
    head = temp->next;
    Tree_Node* returnData = temp->data;
    delete temp;
    return returnData;
}

// Removes the node with data == nodePtr and returns its data
Tree_Node* List::removeValue(Tree_Node* delPtr) {
    // Check if empty list
    assert(head != nullptr);
    
    if (head->next == nullptr) {
        // Single node
        return deleteSingleNode();
    }
    
    // Value at start of list
    if (head->data == delPtr) {
        return removeStart();
    }
    
    List_Node *current = head;
    List_Node *after = current->next;
    
    while (after->data != delPtr && after->next != nullptr) {
        current = after;
        after = current->next;
    }
    
    // Reached value or end of list
    // If value found
    if (after->data == delPtr) {
        // Call removeEnd for special procedure if deleting tail node
        if (after == tail) {
            return removeEnd();
        }
        
        current->next = after->next;
        delete after;
        return delPtr;
    }
    
    // Function failed
    assert(false);
}

// Checks if list is empty
bool List::empty() {
    return head == nullptr;
}

// Encapsulates code to delete the last node of the list
Tree_Node* List::deleteSingleNode() {
    List_Node *temp = tail;
    Tree_Node* returnData = temp->data;
    head = nullptr;
    tail = nullptr;
    delete temp;
    return returnData;
}

// Prints linked list's data values to the screen
//void List::display() {
//    
//    // Create temporary node pointer to move through list
//    node *temp = head;
//    if (temp == nullptr) {
//        cout << "Empty List" << endl;
//    }
//    else {
//        cout << "Printing Linked List" << endl;
//        cout << temp->data << endl;
//        while (temp->next != nullptr) {
//            temp = temp->next;
//            cout << temp->data << endl;
//        }
//    }
//}
