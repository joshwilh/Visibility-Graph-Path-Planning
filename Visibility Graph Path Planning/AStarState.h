//
//  AStarState.h
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 11/6/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#ifndef AStarState_h
#define AStarState_h

#include "preprocessing.h"

// Shallow copies ok (just a pointer, owned elsewhere anyway)
struct State {
    const Vertex *position;
};

// EFFECTS : Returns true if lhs and rhs are the same state
bool operator==(const State &lhs, const State &rhs);

#endif /* AStarState_h */
