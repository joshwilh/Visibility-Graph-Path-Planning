//
//  Tree_Node.cpp
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 11/4/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include "Tree_Node.h"

using namespace std;

// EFFECTS : Returns true if lhs has lower fCost than rhs.
bool operator<(const Tree_Node &lhs, const Tree_Node &rhs) {
    return lhs.fCost < rhs.fCost;
}
