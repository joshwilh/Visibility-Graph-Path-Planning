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

// Encapsulates code to delete the last node of the list
template <typename T>
T* List<T>::deleteSingleNode() {
    List_Node *temp = tail;
    T* returnData = temp->data;
    head = nullptr;
    tail = nullptr;
    delete temp;
    return returnData;
}

//MODIFIES: this
//EFFECTS:  copies all nodes from other to this
template <typename T>
void List<T>::copy_all(const List<T> &other) {
    List_Node *other_ptr = other.head;
    if (!other_ptr) {
        //other is empty
        return;
    }
    // Create first node
    List_Node *first_node = new List_Node;
    first_node->data = other_ptr->data;
    head = first_node;
    ++List_size;

    // Create two pointers to move with linked list as it is created
    List_Node *prev_ptr = first_node;
    List_Node *next_ptr = nullptr;
    
    other_ptr = other_ptr->next;
    
    while (other_ptr) {
        next_ptr = new List_Node;
        next_ptr->data = other_ptr->data;
        prev_ptr->next = next_ptr;
        
        prev_ptr = next_ptr;
        next_ptr = nullptr;
        
        other_ptr = other_ptr->next;
        ++List_size;
    }
    
    //Set the last node's next pointer to null and point last to it
    prev_ptr->next = nullptr;
    tail = prev_ptr;
}

//MODIFIES: this, may invalidate list iterators
//EFFECTS:  removes all nodes
template <typename T>
void List<T>::pop_all() {
    if(!empty()) {
        // Delete all nodes
        List_Node* current = head;
        List_Node* after = head->next;
        
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

// Constructs empty list that DOES NOT own the data
template <typename T>
List<T>::List() :
List(false) {}

// Constructs empty list, allows specification of ownership flag
template <typename T>
List<T>::List(bool owner_flag) :
// Set head and tail to null to avoid garbage value
owner_of_data(owner_flag), head(nullptr), tail(nullptr), List_size(0){}

// Copy Constructor
// NOTE: New list will NOT own the data, regardless of other's ownership
// status
template <typename T>
List<T>::List(const List<T> &other)
: List(false) {
    copy_all(other);
}

// Destructor: Deletes all nodes in list. Also deletes data if owner_of_data
//             is true
template <typename T>
List<T>::~List() {
    pop_all();
}

// MODIFIES: this
// EFFECTS: deep copies rhs list into this
// NOTE: New list will NOT own the data, regardless of rhs' ownership status
template <typename T>
List<T> & List<T>::operator=(const List<T> &rhs) {
    // Check for self-assigment
    if (this == &rhs) { return *this; }
    pop_all();
    owner_of_data = false;
    head = tail = nullptr;
    List_size = 0;
    copy_all(rhs);
    return *this;
}

// Adds a node to the end of the list
template <typename T>
void List<T>::insertEnd(T* nodePtr) {
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
    // Increment size
    ++List_size;
}

// Adds a node to the beginning of the list
template <typename T>
void List<T>::insertStart(T* nodePtr) {
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
    // Increment size
    ++List_size;
}

//// Calls private function of same name
//template <typename T>
//void List<T>::insertByValue(T* nodePtr) {
//    insertByValue(nodePtr, id<T>());
//    // Increment size
//    ++List_size;
//}

// REQUIRES: Type T has an operator overload for <
// EFFECTS : Adds a node to the list based on its type's overload <
//           Node is inserted such that all nodes are sorted smallest to
//           largest from head to tail
template <typename T>
void List<T>::insertByValue(T* nodePtr) {
    // If list empty, insert using insertStart
    if (empty()) {
        insertStart(nodePtr);
        return;
    }
    
    // Check if new node needs to be inserted at start
    if (*nodePtr < *head->data) {
        insertStart(nodePtr);
        return;
    }
    
    // Traverse list
    List_Node *current = head;
    List_Node *after = head->next;
    while (after != nullptr) {
        if (*nodePtr < *after->data) {
            // Found node to insert before (after)
            List_Node *temp = new List_Node;
            temp->data = nodePtr;
            temp->next = after;
            current->next = temp;
            ++List_size;
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
// WARNING: SLOW in this implementation (singly-linked)
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
        List_Node *current = head;
        List_Node *after = current->next;
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
    List_Node *temp = head;
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

// Pre-iterator functions
//// EFFECTS : returns the data member of the first item in the List, returns the
////           null pointer if List is empty
//// NOTE    : Can be used together with nextItem to iterate through a List
//template <typename T>
//T* List<T>::firstItem() const{
//    if (empty()) {
//        return nullptr;
//    }
//    return head->data;
//}
//
//// REQUIRES: currentItem is a pointer to a data member of a List_Node in
////           List
//// EFFECTS : returns the data member of the next pointer in currentItem's
////           List_Node. If currentItem is the last item in the list, returns
////           the first item.
//template <typename T>
//T* List<T>::nextItem(const T* currentItem) const{
//    List_Node* searchPtr = head;
//    while (searchPtr != nullptr) {
//        if (searchPtr->data == currentItem) {
//            // Found item
//            if (searchPtr == tail) {
//                // At end of list
//                return firstItem();
//            }
//            return searchPtr->next->data;
//        }
//        searchPtr = searchPtr->next;
//    }
//
//    // Requires clause broken (currentItem is not in List)
//    assert(false);
//}

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
    
    List_Node* ptr = head;
    
    for (int i = 0; i < index; ++i) {
        ptr = ptr->next;
    }
    
    return ptr->data;
}

// Public constructor. Creates an end Iterator
template <typename T>
List<T>::Iterator::Iterator()
    : nodePtr(nullptr) {}

// Private constructor. Creates an Iterator pointing to the specified
// List_Node.
template <typename T>
List<T>::Iterator::Iterator(List_Node *np)
    : nodePtr(np) {}

// REQUIRES: this is a dereferenceable iterator
// EFFECTS : Returns the element this iterator points to.
template <typename T>
T* & List<T>::Iterator::operator*() const {
    assert(nodePtr);
    return nodePtr->data;
}

// REQUIRES: this is a dereferenceable iterator
// EFFECTS : Increments this iterator to point to the next element.
//           Returns this iterator by reference.
template <typename T>
typename List<T>::Iterator & List<T>::Iterator::operator++() {
    assert(nodePtr);
    nodePtr = nodePtr->next;
    return *this;
}

// REQUIRES: this is a dereferenceable iterator
// EFFECTS : Increments this iterator to point to the next element. If
//           this iterator is pointing to the last element when
//           circularIncrement() is called, points the iterator to
//           begin. Returns this iterator by reference.
// NOTE:     This increment will never make this the end iterator!
template <typename T>
typename List<T>::Iterator &
List<T>::Iterator::circularIncrement(List<T>::Iterator begin) {
    assert(nodePtr);
    if (nodePtr->next == nullptr) {
        *this = begin;
    } else {
        nodePtr = nodePtr->next;
    }
    return *this;
}

// EFFECTS: Returns whether this and rhs are pointing to the same place.
// NOTE:    The result is only meaningful if both are pointing into the
//          same underlying container
template <typename T>
bool List<T>::Iterator::operator==(Iterator rhs) const {
    return nodePtr == rhs.nodePtr;
}

// EFFECTS: Returns whether this and rhs are NOT pointing to the same
//          place.
// NOTE:    The result is only meaningful if both are pointing into the
//          same underlying container
template <typename T>
bool List<T>::Iterator::operator!=(Iterator rhs) const {
    return nodePtr != rhs.nodePtr;
}

// EFFECTS: Returns an Iterator pointing to the first element in List
template <typename T>
typename List<T>::Iterator List<T>::begin() {
    return Iterator(head);
}

// EFFECTS: Returns an Iterator pointing to the first element in List
template <typename T>
typename List<T>::Iterator List<T>::begin() const {
    return Iterator(head);
}

// EFFECTS: Returns an Iterator pointing to one "past the end" of List
template <typename T>
typename List<T>::Iterator List<T>::end() {
    return Iterator();
}

// EFFECTS: Returns an Iterator pointing to one "past the end" of List
template <typename T>
typename List<T>::Iterator List<T>::end() const {
    return Iterator();
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
    
        // Traversal by Iterator
        typename List<T>::Iterator end = L.end();
        int i = 0;
        for (typename List<T>::Iterator it = L.begin(); it != end; ++it) {
            os << "Item " << i << ": " << **it << endl;
            ++i;
        }
    }
    
    return os;
}
