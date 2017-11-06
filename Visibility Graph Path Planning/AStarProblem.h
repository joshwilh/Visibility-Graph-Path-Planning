//
//  AStarProblem.h
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 11/4/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//
//  This is the domain-dependent part of the A-Star Search. Each new problem
//  needs to change the type of object held by State, as well as the functions
//  in Problem.

#ifndef AStarProblem_h
#define AStarProblem_h

#include <vector>
#include <iostream>
#include "preprocessing.h"
#include "Tree_Node.h"

// Shallow copies ok (just a pointer, owned elsewhere anyway)
struct State {
    const Vertex *position;
};

// EFFECTS : Returns true if lhs and rhs are the same state
bool operator==(const State &lhs, const State &rhs);

class Problem {
private:
    const Graph &graph;
    State startState;
    State goalState;
    
public:
    // REQUIRES: graphIn has been pre-processed and is ready for searching
    //           startStateIn, goalStateIn are valid states in graphIn
    // EFFECTS : Sets the graph, startState, and goalState
    Problem(const Graph& graphIn, State startStateIn, State goalStateIn);
    
    // EFFECTS : Dynamically creates a root node for the search tree based on
    //           startState
    Tree_Node* getRootNode() const;
    
    // EFFECTS: Returns true if goalState == checkNode->state, false otherwise
    bool goalTest(const Tree_Node* checkNode) const;
    
    // EFFECTS : Returns the straight-line-distance from the currentState to the
    //           goalState
    double heuristic(const State &currentState) const;
    
    // EFFECTS : Adds the states that can be traveled to from currentState to
    //           possibleStates, with the associated path costs in pathCosts
    void expandOptions(std::vector<State> &possibleStates,
                        std::vector<double> &pathCosts,
                        const State &currentState) const;
    
    // MODIFIES: os
    // EFFECTS : Prints the solution vector to os. Prints "Failure" if solution
    //           is empty.
    //           Note: This requires the solution vector to be printed in
    //           reverse order
    void printSolution(std::vector<State> &solution, std::ostream &os) const;
};

#endif /* AStarProblem_h */
