//
//  Tree.cpp
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "Tree.h"

using namespace std;

// Default constructor
Tree::Tree(Tree_Node rootNode) {
    Tree_Node* temp = new Tree_Node;
    *temp = rootNode;
    root = temp;
}

// Destructor
Tree::~Tree() {
    deleteDown(root);
}

void Tree::deleteDown(Tree_Node* nodeToDelete) {
    // Remove one node off of child list each loop, recursively delete all
    // children
    while (!nodeToDelete->childList.empty()) {
        deleteDown(nodeToDelete->childList.removeStart());
    }
    delete nodeToDelete;
    nodeToDelete = nullptr;
}

// Returns address of root node of tree
Tree_Node* Tree::getRootAddress() {
    return root;
}

// Adds new node to tree as child of parent node
Tree_Node* Tree::addNode(Tree_Node newNode) {
    Tree_Node* temp = new Tree_Node;
    *temp = newNode;
    
    newNode.parent->childList.insertEnd(temp);
    
    return temp;
}

// REQUIRES: nodePtr points to a valid node that is NOT the root node
// Removes specified node from tree. Also deletes all parent nodes if their
// child lists become empty
void Tree::removeNode(Tree_Node* nodePtr) {
    if (!nodePtr->childList.empty()) {
        cout << "Error, cannot delete node with children." << endl;
        assert(false);
    }
    Tree_Node* tempParent = nodePtr->parent;
    
    tempParent->childList.removeValue(nodePtr);
    
    // Do not check/delete parent if tempParent is root node (bug fix)
    // Root node will be deleted by destructor
    if (tempParent->parent != nullptr) {
    
        if(tempParent->childList.empty()) {
            // Recursively delete all parents that lose last child node
            removeNode(tempParent);
        }
    }
    delete nodePtr;
}