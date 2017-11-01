//
//  List.h
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#ifndef LIST_H
#define LIST_H

#include <stdio.h>

// Forward declaration
struct Tree_Node;

// Each element of List
template <typename T>
// T is type of data to be stored in each List_Node
struct List_Node {
    T* data;
    List_Node* next;
};

// id struct will allow determination of type of T
template <typename T> struct id { typedef T type; };

// Linked list class
template <typename T>
// T is type of data to be stored in each List_Node
class List {
private:
    // Determines whether the List will delete all data in destructor
    bool owner_of_data;
    
    // Node pointers that point to first and last nodes of list
    List_Node<T> *head, *tail;
    
    // The size of the list
    int List_size;
    
    // Encapsulates code to delete the last node of the list
    T* deleteSingleNode();
    
    // FOR TYPE T == Tree_Node:
    //      Adds a node to the list based on its f-cost
    //      Node is inserted such that all nodes are sorted by f-cost
    //      smallest to largest from head to tail
    void insertByValue(T* nodePtr, id<Tree_Node>);
    
public:
    // Constructs empty list that DOES NOT own the data
    List();
    
    // Constructs empty list, allows specification of ownership flag
    List(bool owner_flag);
    
    // Destructor: Deletes all nodes in list. Also deletes data if owner_of_data
    //             is true
    ~List();
    
    // Adds a node to the end of the list
    void insertEnd(T* nodePtr);
    
    // Adds a node to the beginning of the list
    void insertStart(T* nodePtr);
    
    // TODO: Improve with <, > operator overload
    // Calls private function of same name
    void insertByValue(T* nodePtr);
    
    // Removes the node at the end of the list and returns its data
    // WARNING: SLOW in this implementation
    T* removeEnd();
    
    // Removes the node at the beginning of the list and returns its data
    T* removeStart();
    
    // Removes the node with data == delPtr and returns its data
    T* removeValue(T* delPtr);
    
    // EFFECTS : returns the data member of the first item in the List, returns
    //           the null pointer if List is empty
    // NOTE    : Can be used together with nextItem to iterate through a List
    T* firstItem() const;
    
    // REQUIRES: currentItem is a pointer to a data member of a List_Node in
    //           List
    // EFFECTS : returns the data member of the next pointer in currentItem's
    //           List_Node. If currentItem is the last item in the list, returns
    //           the first item.
    T* nextItem(const T* currentItem) const;
    
    // Returns the size of the list
    int size() const;
    
    // Checks if list is empty
    bool empty() const;
    
    // REQUIRES: 0 <= index < size
    // EFFECTS : returns the data of the List_Node at position index in the List
    T* at(int index) const;
};

// REQUIRES: type T has an overloaded operator<<
// MODIFIES: os
// EFFECTS : Prints the list to os
template <typename T>
std::ostream & operator<<(std::ostream &os, const List<T> &L);

// Necessary to separate interface from implementation with templates
#include "List.tpp"

#endif /* LIST_H */
