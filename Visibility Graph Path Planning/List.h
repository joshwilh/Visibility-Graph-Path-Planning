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

// Linked list class
template <typename T>
// T is type of data to be stored in each List_Node
class List {
private:
    
    // Each element of List
    struct List_Node {
        T* data;
        List_Node* next;
    };
    
    // Determines whether the List will delete all data in destructor
    bool owner_of_data;
    
    // Node pointers that point to first and last nodes of list
    List_Node *head, *tail;
    
    // The size of the list
    int List_size;
    
    // Encapsulates code to delete the last node of the list
    T* deleteSingleNode();
    
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
    
    // REQUIRES: Type T has an operator overload for <
    // EFFECTS : Adds a node to the list based on its type's overload <
    //           Node is inserted such that all nodes are sorted smallest to
    //           largest from head to tail
    void insertByValue(T* nodePtr);
    
    // Removes the node at the end of the list and returns its data
    // WARNING: SLOW in this implementation (singly-linked)
    T* removeEnd();
    
    // Removes the node at the beginning of the list and returns its data
    T* removeStart();
    
    // Removes the node with data == delPtr and returns its data
    T* removeValue(T* delPtr);
    
    // Pre-iterator functions
//    // EFFECTS : returns the data member of the first item in the List, returns
//    //           the null pointer if List is empty
//    // NOTE    : Can be used together with nextItem to iterate through a List
//    T* firstItem() const;
//
//    // REQUIRES: currentItem is a pointer to a data member of a List_Node in
//    //           List
//    // EFFECTS : returns the data member of the next pointer in currentItem's
//    //           List_Node. If currentItem is the last item in the list, returns
//    //           the first item.
//    T* nextItem(const T* currentItem) const;
    
    // Returns the size of the list
    int size() const;
    
    // Checks if list is empty
    bool empty() const;
    
    // REQUIRES: 0 <= index < size
    // EFFECTS : returns the data of the List_Node at position index in the List
    T* at(int index) const;
    
    // An Iterator can be used to iterate through a List
    class Iterator {
        friend class List;
        
    public:
        // Public constructor. Creates an end Iterator
        Iterator();
        
        // REQUIRES: this is a dereferenceable iterator
        // EFFECTS : Returns the element this iterator points to.
        T* & operator*() const;
        
        // REQUIRES: this is a dereferenceable iterator
        // EFFECTS : Increments this iterator to point to the next element.
        //           Returns this iterator by reference.
        Iterator & operator++();
        
        // REQUIRES: this is a dereferenceable iterator
        // EFFECTS : Increments this iterator to point to the next element. If
        //           this iterator is pointing to the last element when
        //           circularIncrement() is called, points the iterator to
        //           begin. Returns this iterator by reference.
        // NOTE:     This increment will never make this the end iterator!
        Iterator & circularIncrement(Iterator begin);
        
        // EFFECTS: Returns whether this and rhs are pointing to the same place.
        // NOTE:    The result is only meaningful if both are pointing into the
        //          same underlying container
        bool operator==(Iterator rhs) const;
        
        // EFFECTS: Returns whether this and rhs are NOT pointing to the same
        //          place.
        // NOTE:    The result is only meaningful if both are pointing into the
        //          same underlying container
        bool operator!=(Iterator rhs) const;
    
    private:
        // Private constructor. Creates an Iterator pointing to the specified
        // List_Node.
        Iterator(List_Node *np);
        
        List_Node *nodePtr;
    };
    
    // EFFECTS: Returns an Iterator pointing to the first element in List
    Iterator begin();
    
    // EFFECTS: Returns an Iterator pointing to the first element in List
    Iterator begin() const;
    
    // EFFECTS: Returns an Iterator pointing to one "past the end" of List
    Iterator end();
    
    // EFFECTS: Returns an Iterator pointing to one "past the end" of List
    Iterator end() const;
};

// REQUIRES: type T has an overloaded operator<<
// MODIFIES: os
// EFFECTS : Prints the list to os
template <typename T>
std::ostream & operator<<(std::ostream &os, const List<T> &L);

// Necessary to separate interface from implementation with templates
#include "List.tpp"

#endif /* LIST_H */
