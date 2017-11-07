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
#include "AStarState.h"

// Node data structure
struct Tree_Node {
    
    // Current state (NOTE: State struct defined in Problem.h)
    State state;
    
    // Node that generated this node
    Tree_Node *parent;
    
    // List of children of this node (non-owner)
    List<Tree_Node> childList;
    
    /* Not currently used
    // Operator that allowed move to this node
    int prevOperator;
     */
    
    // Depth of node
    int depth;
    
    // Cost of path from initial state to this node
    double pathCost;
    
    // Sum of pathCost and heuristic
    double fCost;
};

// EFFECTS : Returns true if lhs has lower fCost than rhs.
bool operator<(const Tree_Node &lhs, const Tree_Node &rhs);

#endif /* Tree_Node_h */
