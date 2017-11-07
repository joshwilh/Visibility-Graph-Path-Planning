//
//  AStarState.cpp
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 11/6/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include "AStarState.h"

using namespace std;

// EFFECTS : Returns true if lhs and rhs are the same state
bool operator==(const State &lhs, const State &rhs) {
    return (*(lhs.position) == *(rhs.position));
}
