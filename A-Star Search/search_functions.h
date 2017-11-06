//
//  search_functions.h
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#ifndef SEARCH_FUNCTIONS_H
#define SEARCH_FUNCTIONS_H

#include "Tree.h"
#include "AStarProblem.h"

// REQUIRES: p is a valid Problem, solution is empty
// MODIFIES: solution
// EFFECTS : Performs an A-star search on the provided problem.
//           solution becomes empty vector if failure, solution path if success
//           returns the path cost of the solution if success, -1 if failure
double AStarSearch(const Problem &p, std::vector<State> &solution);

// MODIFIES: searchTree, openList, expandedNode
// EFFECTS : Adds all valid expanisions of expandedNode to the tree and the open
//           list. Will not add nodes with states that are in the closedList. If
//           expandedNode cannot be expanded, deletes expandedNode in tree.
void expand(const Problem &p, Tree &searchTree,
            const std::vector<State> &closedList, List<Tree_Node> &openList,
            Tree_Node* expandedNode);

// REQUIRES: goalNode is a valid pointer to a node. searchTree is a valid tree
//           with a path from goalNode back to root through parent pointers.
//           solution is empty
// MODIFIES: solution
// EFFECTS : Fills solution with States that specify the path from
//           FINISH to START
void findSoln(const Tree &searchTree, const Tree_Node* goalNode,
              std::vector<State> &solution);

#endif /* SEARCH_FUNCTIONS_H */
