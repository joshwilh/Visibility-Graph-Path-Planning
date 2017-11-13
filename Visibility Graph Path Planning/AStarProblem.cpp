//
//  AStarProblem.cpp
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 11/4/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include "AStarProblem.h"

using namespace std;

// REQUIRES: graphIn has been pre-processed and is ready for searching
//           startStateIn, goalStateIn are valid states in graphIn
// EFFECTS : Sets the graph, startState, and goalState
Problem::Problem(const Graph& graphIn, State startStateIn, State goalStateIn)
: graph(graphIn), startState(startStateIn), goalState(goalStateIn) {}

// EFFECTS : Dynamically creates a root node for the search tree based on
//           startState
Tree_Node* Problem::getRootNode() const {
    List<Tree_Node> rootChildren(false);
    // Shallow copy of an empty list should be ok
    Tree_Node* root = new Tree_Node{startState, nullptr, rootChildren, 0, 0,
                                    heuristic(startState)};
    return root;
}

// EFFECTS: Returns true if goalState == checkNode->state, false otherwise
bool Problem::goalTest(const Tree_Node* checkNode) const {
    return goalState == checkNode->state;
}

// EFFECTS : Returns the straight-line-distance from the currentState to the
//           goalState
double Problem::heuristic(const State &currentState) const {
    return distanceFormula(*currentState.position, *goalState.position,
                           DIMENSIONS);
}

// EFFECTS : Adds the states that can be traveled to from currentState to
//           possibleStates, with the associated path costs in pathCosts
void Problem::expandOptions(vector<State> &possibleStates,
                            vector<double> &pathCosts,
                            const State &currentState) const {
    // Loop through graph's connections

    // Traversal by Iterator
    List<Edge>::Iterator end = graph.connections.end();
    for (List<Edge>::Iterator currentEdge = graph.connections.begin();
         currentEdge != end; ++currentEdge) {
        // Check if currentState is one of the endpoints
        if (currentState.position == (*currentEdge)->v1) {
            possibleStates.push_back(State{(*currentEdge)->v2});
            pathCosts.push_back((*currentEdge)->length);
        } else if (currentState.position == (*currentEdge)->v2) {
            possibleStates.push_back(State{(*currentEdge)->v1});
            pathCosts.push_back((*currentEdge)->length);
        }
    }
}

// MODIFIES: os
// EFFECTS : Prints the solution vector to os. Prints "Failure" if solution
//           is empty.
//           Note: This requires the solution vector to be printed in
//           reverse order
void Problem::printSolution(vector<State> &solution, ostream &os) const {
    // Prints the solution vector (or failure) to os
    if (solution.empty()) {
        cout << "Failure" << endl;
    }
    else {
        cout << "Success!" << endl;
        // Loop backwards through solution vector
        for (int i = (int) solution.size() - 1, j = 1; i >= 0; --i, ++j) {
            os << j << ": " << *solution.at(i).position << endl;
        }
        os << endl;
    }
}
