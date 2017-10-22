//
//  List.tpp
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <iostream>
#include <cassert>
//#include "Tree_Node.h"
#include "List.h"
using namespace std;

// Constructs empty list
template <typename T>
List<T>::List() {
    // Set to null to avoid garbage value
    head = nullptr;
    tail = nullptr;
}

// Destructor: Deletes all nodes in list
template <typename T>
List<T>::~List() {
    if(!empty()) {
        // Delete all nodes
        List_Node<T>* current = head;
        List_Node<T>* after = head->next;
        
        delete current;
        while (after != nullptr) {
            current = after;
            after = after->next;
            delete current;        
        }
    }
}

// Adds a node to the end of the list
template <typename T>
void List<T>::insertEnd(T* nodePtr) {
    List_Node<T> *temp = new List_Node<T>;
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
template <typename T>
void List<T>::insertStart(T* nodePtr) {
    List_Node<T> *temp = new List_Node<T>;
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

// Calls private function of same name
template <typename T>
void List<T>::insertByValue(T* nodePtr) {
    insertByValue(nodePtr, id<T>());
}

// FOR TYPE T == Tree_Node:
//      Adds a node to the list based on its f-cost
//      Node is inserted such that all nodes are sorted by f-cost
//      smallest to largest from head to tail
template <typename T>
void List<T>::insertByValue(T* nodePtr, id<Tree_Node>) {
    // If list empty, insert using insertStart
    if (empty()) {
        insertStart(nodePtr);
        return;
    }
    
    List_Node<T> *temp = new List_Node<T>;
    temp->data = nodePtr;
    
    int newFCost = nodePtr->fCost;
    
    // Check if new node needs to be inserted at start
    if (newFCost < head->data->fCost) {
        insertStart(nodePtr);
        return;
    }
    
    // Traverse list
    List_Node<T> *current = head;
    List_Node<T> *after = head->next;
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
template <typename T>
T* List<T>::removeEnd() {
    // Check if empty list
    assert(head != nullptr);
    
    if (head->next == nullptr) {
        // Single node
        return deleteSingleNode();
    }
    else {
        // Traverse list, end when "current" is second to last node and "after" is tail
        List_Node<T> *current = head;
        List_Node<T> *after = current->next;
        while (after->next != nullptr) {
            current = after;
            after = current->next;
        }
        current->next = nullptr;
        tail = current;
        T* returnData = after->data;
        delete after;
        return returnData;
    }
}

// Removes the node at the beginning of the list and returns its data
template <typename T>
T* List<T>::removeStart() {
    // Check if empty list
    assert(head != nullptr);
    
    if (head->next == nullptr) {
        // Single node
        return deleteSingleNode();
    }
    List_Node<T> *temp = head;
    head = temp->next;
    T* returnData = temp->data;
    delete temp;
    return returnData;
}

// Removes the node with data == delPtr and returns its data
template <typename T>
T* List<T>::removeValue(T* delPtr) {
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
    
    List_Node<T> *current = head;
    List_Node<T> *after = current->next;
    
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

// REQUIRES: currentItem is a pointer to a data member of a List_Node in
//           List
// EFFECTS : returns the data member of the next pointer in currentItem's
//           List_Node. Returns the null pointer if currentItem is the last item
//           in List
template <typename T>
T* List<T>::nextItem(const T* currentItem) {
    List_Node<T>* searchPtr = head;
    while (searchPtr != nullptr) {
        if (searchPtr->data == currentItem) {
            // Found item
            if (searchPtr == tail) {
                // At end of list
                return nullptr;
            }
            return searchPtr->next->data;
        }
        searchPtr = searchPtr->next;
    }
    
    // Requires clause broken (currentItem is not in List)
    assert(false);
}

// Checks if list is empty
template <typename T>
bool List<T>::empty() {
    return head == nullptr;
}

// Encapsulates code to delete the last node of the list
template <typename T>
T* List<T>::deleteSingleNode() {
    List_Node<T> *temp = tail;
    T* returnData = temp->data;
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
