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

// Constructs empty list that DOES NOT own the data
template <typename T>
List<T>::List() :
List(false) {}

// Constructs empty list, allows specification of ownership flag
template <typename T>
List<T>::List(bool owner_flag) :
// Set head and tail to null to avoid garbage value
owner_of_data(owner_flag), head(nullptr), tail(nullptr), List_size(0){}

// Destructor: Deletes all nodes in list. Also deletes data if owner_of_data
//             is true
template <typename T>
List<T>::~List() {
    if(!empty()) {
        // Delete all nodes
        List_Node<T>* current = head;
        List_Node<T>* after = head->next;
        
        if (owner_of_data) {
            delete current->data;
        }
        
        delete current;
        while (after != nullptr) {
            current = after;
            after = after->next;
            
            if (owner_of_data) {
                delete current->data;
            }
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
    // Increment size
    ++List_size;
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
    // Increment size
    ++List_size;
}

// Calls private function of same name
template <typename T>
void List<T>::insertByValue(T* nodePtr) {
    insertByValue(nodePtr, id<T>());
    // Increment size
    ++List_size;
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
    
    // Decrement size
    --List_size;
    
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
    
    // Decrement size
    --List_size;
    
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
    
    // Decrement size
    --List_size;
    
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

// EFFECTS : returns the data member of the first item in the List, returns the
//           null pointer if List is empty
// NOTE    : Can be used together with nextItem to iterate through a List
template <typename T>
T* List<T>::firstItem() const{
    if (empty()) {
        return nullptr;
    }
    return head->data;
}

// REQUIRES: currentItem is a pointer to a data member of a List_Node in
//           List
// EFFECTS : returns the data member of the next pointer in currentItem's
//           List_Node. If currentItem is the last item in the list, returns
//           the first item.
template <typename T>
T* List<T>::nextItem(const T* currentItem) const{
    List_Node<T>* searchPtr = head;
    while (searchPtr != nullptr) {
        if (searchPtr->data == currentItem) {
            // Found item
            if (searchPtr == tail) {
                // At end of list
                return firstItem();
            }
            return searchPtr->next->data;
        }
        searchPtr = searchPtr->next;
    }
    
    // Requires clause broken (currentItem is not in List)
    assert(false);
}

// Returns the size of the list
template <typename T>
int List<T>::size() const{
    return List_size;
}

// Checks if list is empty
template <typename T>
bool List<T>::empty() const{
    return head == nullptr;
}

// REQUIRES: 0 <= index < size
// EFFECTS : returns the data of the List_Node at position index in the List
template <typename T>
T* List<T>::at(int index) const {
    // Check requires clause
    assert(index >= 0 && index < List_size);
    
    List_Node<T>* ptr = head;
    
    for (int i = 0; i < index; ++i) {
        ptr = ptr->next;
    }
    
    return ptr->data;
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

// REQUIRES: type T has an overloaded operator<<
// MODIFIES: os
// EFFECTS : Prints the list to os
template <typename T>
std::ostream & operator<<(std::ostream &os, const List<T> &L) {
    
    if (L.empty()) {
        os << "Empty List" << endl;
    }
    else {
        cout << "Printing Linked List" << endl;
        
        const T* item = L.firstItem();
        
        for (int i = 0; i < L.size(); ++i) {
            os << "Item " << i << ": " << *item << endl;
            item = L.nextItem(item);
        }
    }
    
    return os;
}
