//
//  Tree_Node.h
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#ifndef Tree_Node_h
#define Tree_Node_h

#include <vector>
#include <string>
#include "List.h"

// Node data structure
struct Tree_Node {
    
    // Current Location - first letter of city (must be UPPER_CASE)
    char state;
    
    // Node that generated this node
    Tree_Node *parent;
    
    // List of children of this node
    List childList;
    
    /* Not relevant to this problem
    // Operator that allowed move to this node
    int prevOperator;
     */
    
    // Depth of node
    int depth;
    
    // Cost of path from initial state to this node
    // For this problem: Road distance traveled in km
    int pathCost;
    
    // Sum of pathCost and Straight-Line-Distance to Bucharest heuristic
    int fCost;
};


#endif /* Tree_Node_h */
