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
#include "List_Node.h"

// Forward declaration
struct Tree_Node;

// Linked list class
class List {
private:
    // Nodes that point to first and last nodes of list
    List_Node *head, *tail;
    
    // Encapsulates code to delete the last node of the list
    Tree_Node* deleteSingleNode();
    
public:
    // Constructs empty list
    List();
    
    // Destructor: Deletes all nodes in list
    ~List();
    
    // Adds a node to the end of the list
    void insertEnd(Tree_Node* nodePtr);
    
    // Adds a node to the beginning of the list
    void insertStart(Tree_Node* nodePtr);
    
    // Adds a node to the list based on its f-cost
    // Node is inserted such that all nodes are sorted by f-cost
    // smallest to largest from head to tail
    void insertByValue(Tree_Node* nodePtr);
    
    // Removes the node at the end of the list and returns its data
    // WARNING: SLOW in this implementation
    Tree_Node* removeEnd();
    
    // Removes the node at the beginning of the list and returns its data
    Tree_Node* removeStart();
    
    // Removes the node with data == nodePtr and returns its data
    Tree_Node* removeValue(Tree_Node* nodePtr);
    
    // Checks if list is empty
    bool empty();
    
    // Prints list (currently not implemented)
    // Tree_Node* display();
};

#endif /* LIST_H */
