//
//  Tree.h
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include "Tree_Node.h"

// Tree class
class Tree {
private:
    // Points to root node of tree
    Tree_Node* root;
    
    // Added to monitor A* memory usage
    int treeSize;
    
    // Recursive function for tree deletion used in destructor
    void deleteDown(Tree_Node* nodeToDelete);
    
public:
    // Constructor
    Tree(Tree_Node *rootNode);
    
    // Destructor
    ~Tree();
    
    // Adds new node to tree as child of parent node, increments size
    void addNode(Tree_Node *newNode);
    
    // REQUIRES: nodePtr points to a valid node that is NOT the root node
    // Removes specified node from tree. Also deletes all parent nodes if their
    // child lists become empty. Decrements size.
    void removeNode(Tree_Node* nodePtr);
    
    // Returns the size of the tree
    int size();
};

#endif /* TREE_H */
