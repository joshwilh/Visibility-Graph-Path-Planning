//
//  search_functions.h
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#ifndef SEARCH_FUNCTIONS_H
#define SEARCH_FUNCTIONS_H

#include <stdio.h>
#include <vector>
#include <string>
#include "Tree_Node.h"
#include "Tree.h"
#include "List.h"

// REQUIRES: startCity is the first letter of a city in the problem
// MODIFIES: solution, distance
// EFFECTS : Performs an A-start search on the Romanian cities problem.
//           solution becomes empty vector if failure, solution path if success
//           distance becomes the distance traveled on the solution path
//           distance becomes -1 for failure
void AStarSearch(char startCity, std::vector<char> &solution, int &distance);

// REQUIRES: checkNode points to a valid Tree_Node
// EFFECTS:  Returns true if checkNode->state == 'B', false otherwise
bool goalTest(const Tree_Node* checkNode);

// EFFECTS: Adds all valid expanisions of expandedNode to the tree and the open
//          list. Will not add nodes with states that are in the closedList. If
//          expandedNode cannot be expanded, deletes expandedNode in tree.
void expand(Tree &searchTree, std::vector<char> closedList,
            List &openList, Tree_Node* expandedNode);

// REQUIRES: goalNode is a valid pointer to a node. searchTree is a valid tree
//           with a path from goalNode back to root through parent pointers
// EFFECTS : Returns a vector of ints that specify the path from FINISH to START
std::vector<char> findSoln(Tree &searchTree, const Tree_Node* goalNode);


// REQUIRES: currentCity is the first letter of a city in the problem.
//           cities and distances are empty vectors.
// MODIFIES: cities, distances
// EFFECTS : Places all possible city connections from currentCity into cities,
//           and all corresponding road distances into distances, with
//           corresponding entries at the same index.
void cityConnections(std::vector<char> &cities,
                     std::vector<int> &distances,
                     const char &currentCity);

// REQUIRES: city is the first letter of a city in the problem.
// EFFECTS : returns the straight-line-distance between city and Bucharest
int straightLineDist(char city);

#endif /* SEARCH_FUNCTIONS_H */
