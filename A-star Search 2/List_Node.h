//
//  List_Node.h
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <stdio.h>

struct Tree_Node;

// Node data structure
struct List_Node {
    Tree_Node* data;
    List_Node* next;
};

#endif /* LIST_NODE_H */
