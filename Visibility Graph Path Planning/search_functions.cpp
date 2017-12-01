//
//  search_functions.cpp
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <vector>
#include <cassert>
#include "search_functions.h"
#include "Tree_Node.h"
#include "Tree.h"
#include "List.h"

using namespace std;


// REQUIRES: p is a valid Problem, solution is empty
// MODIFIES: solution
// EFFECTS : Performs an A-star search on the provided problem.
//           solution becomes empty vector if failure, solution path if success
//           nodesExpanded is incremented by the number of nodes expanded during
//              the A* search
//           returns the path cost of the solution if success, -1 if failure
double AStarSearch(const Problem &p, std::vector<State> &solution,
                   int &nodesExpanded) {
    // Check requires clause
    assert(solution.empty());
    
    // Create tree, open list, and closed list
    Tree mainTree(p.getRootNode());
    
    List<Tree_Node> openList(false);
    openList.insertByValue(p.getRootNode());
    
    vector<State> closedList;
    
    // Continue searching while open list is not empty
    while (!openList.empty()) {
        // Grab next node to check (smallest f-cost always at start)
        Tree_Node* nodeChoice = openList.removeStart();
        
        if (p.goalTest(nodeChoice)) {
            // Found solution!
            findSoln(mainTree, nodeChoice, solution);
            return nodeChoice->pathCost;
        }
        
        // Add node choice to closed list now to avoid allowing path to itself
        closedList.push_back(nodeChoice->state);
        
        ++nodesExpanded;
        expand(p, mainTree, closedList, openList, nodeChoice);
    }
    
    // Failure
    return -1;
}

// MODIFIES: searchTree, openList, expandedNode
// EFFECTS : Adds all valid expanisions of expandedNode to the tree and the open
//           list. Will not add nodes with states that are in the closedList. If
//           expandedNode cannot be expanded, deletes expandedNode in tree.
void expand(const Problem &p, Tree &searchTree,
            const std::vector<State> &closedList, List<Tree_Node> &openList,
            Tree_Node* expandedNode) {
    
    // Switch set to true if child found and added
    bool nodeExpanded = false;
    
    // First, create vector of all possible states and distances from current
    // city
    vector<State> possibleStates;
    vector<double> pathCosts;
    
    // Get possible expansions from the AStarProblem
    p.expandOptions(possibleStates, pathCosts, expandedNode->state);
    
    // Switch to decide whether to add state
    bool addThisState = true;
    
    // Build new nodes and place in vector
    for (int i = 0; i < (int) possibleStates.size(); i++) {
        
        addThisState = true;
        
        // Check closedList for this State. If found, don't expand
        for (int j = 0; j < (int) closedList.size(); j++) {
            if (possibleStates.at(i) == closedList.at(j)) {
                // This state has been visited before
                addThisState = false;
                break;
            }
        }
        // Check openList for this State. If found, don't expand.
        if (addThisState) {
            // Traversal by Iterator
            auto end = openList.end();
            for (auto it = openList.begin(); it != end; ++it) {
                if (possibleStates.at(i) == (*it)->state) {
                    // This state is already on the open list
                    addThisState = false;
                    break;
                }
            }
        }
        
        if (addThisState) {
            // Create new node, add to tree and open list
            List<Tree_Node> newNodeChildren(false);
            double newPathCost = expandedNode->pathCost + pathCosts.at(i);
            double newFCost = newPathCost + p.heuristic(possibleStates.at(i));
            // Shallow copy of an empty list should be ok
            // newNode will be owned by mainTree
            Tree_Node *newNode = new Tree_Node{possibleStates.at(i),
                expandedNode, newNodeChildren, expandedNode->depth + 1,
                newPathCost, newFCost};
            
            searchTree.addNode(newNode);
            openList.insertByValue(newNode); // inserts by f-cost
            
            // expandedNode has at least one child
            nodeExpanded = true;
        }
    }
    
    // If node was not expanded, remove from tree
    // Do remove node if it is the root node that cannot be expanded!
    //    Edge case for cases in which the root has no expansion
    //    (depth-limit = 0, no valid operators, etc). Bug fix.
    if (!nodeExpanded && expandedNode->parent != nullptr) {
        searchTree.removeNode(expandedNode);
        return;
    }
    
    return;
}

// REQUIRES: goalNode is a valid pointer to a node. searchTree is a valid tree
//           with a path from goalNode back to root through parent pointers.
//           solution is empty
// MODIFIES: solution
// EFFECTS : Fills solution with States that specify the path from
//           FINISH to START
void findSoln(const Tree &searchTree, const Tree_Node* goalNode,
              std::vector<State> &solution) {
    // Check requires clause
    assert(solution.empty());
    
    solution.push_back(goalNode->state);
    
    // Add all ancestor states to vector (NOTE: currently in reverse order)
    while (goalNode->parent != nullptr) {
        solution.push_back(goalNode->parent->state);
        goalNode = goalNode->parent;
    }
}
