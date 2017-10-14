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


// REQUIRES: startCity is the first letter of a city in the problem
// MODIFIES: solution, distance
// EFFECTS : Performs an A-start search on the Romanian cities problem.
//           solution becomes empty vector if failure, solution path if success
//           distance becomes the distance traveled on the solution path
//           distance becomes -1 for failure
void AStarSearch(char startCity, vector<char> &solution, int &distance) {
    // Create tree, open list, and closed list
    List rootChildren;
    Tree_Node rootNode = {startCity, nullptr, rootChildren, 0, 0,
        straightLineDist(startCity)};
    Tree mainTree(rootNode);
    
    List openList;
    Tree_Node* rootPtr = mainTree.getRootAddress();
    openList.insertByValue(rootPtr);
    
    vector<char> closedList;
    
    // Continue searching while open list is not empty
    while (!openList.empty()) {
        // Grab next node to check (smallest f-cost always at start)
        Tree_Node* nodeChoice = openList.removeStart();
        
        if (goalTest(nodeChoice)) {
            // Found solution!
            solution = findSoln(mainTree, nodeChoice);
            distance = nodeChoice->pathCost;
            return;
        }
        
        // Add node choice to closed list now to avoid allowing path to itself
        closedList.push_back(nodeChoice->state);
        
        expand(mainTree, closedList, openList, nodeChoice);
    }
    
    // Failure
    vector<char> emptyVector = {};
    solution = emptyVector;
    distance = -1;
    return;
}

// REQUIRES: checkNode points to a valid Tree_Node
// EFFECTS:  Returns true if checkNode->state == 'B', false otherwise
bool goalTest(const Tree_Node* checkNode) {
    return checkNode->state == 'B';
}

// EFFECTS: Adds all valid expanisions of expandedNode to the tree and the open
//          list. Will not add nodes with states that are in the closedList. If
//          expandedNode cannot be expanded, deletes expandedNode in tree.
void expand(Tree &searchTree, vector<char> closedList,
            List &openList, Tree_Node* expandedNode) {
    
    // Switch set to true if child found and added
    bool nodeExpanded = false;
    
    // First, create vector of all possible states and distances from current
    // city
    vector<char> possibleStates;
    vector<int> cityDistances;
    
    // Get possible expansions and distances from function
    cityConnections(possibleStates, cityDistances, expandedNode->state);
    
    // Switch to decide whether to add state
    bool addThisState = true;
    
    // Build new nodes and place in vector
    for (int i = 0; i < possibleStates.size(); i++) {
        
        addThisState = true;
        
        for (int j = 0; j < closedList.size(); j++) {
            if (possibleStates.at(i) == closedList.at(j)) {
                // This state has been visited before
                addThisState = false;
                break;
            }
        }
        
        if (addThisState) {
            // Create new node, add to tree and open list
            List newNodeChildren;
            int newPathCost = expandedNode->pathCost + cityDistances.at(i);
            int newFCost = newPathCost + straightLineDist(possibleStates.at(i));
            Tree_Node newNode = {possibleStates.at(i), expandedNode,
                newNodeChildren, expandedNode->depth + 1,
                newPathCost, newFCost};
            
            Tree_Node* addNodePtr = searchTree.addNode(newNode);
            openList.insertByValue(addNodePtr); // inserts by f-cost
            
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
//           with a path from goalNode back to root through parent pointers
// EFFECTS : Returns a vector of ints that specify the path from FINISH to START
vector<char> findSoln(Tree &searchTree, const Tree_Node* goalNode) {
    vector<char> returnVector = {goalNode->state};
    
    // Add all ancestor states to vector (NOTE: currently in reverse order)
    while (goalNode->parent != nullptr) {
        returnVector.push_back(goalNode->parent->state);
        goalNode = goalNode->parent;
    }
    
    return returnVector;
}

// REQUIRES: currentCity is the name of a city in the problem.
//           cities and distances are empty vectors.
// MODIFIES: cities, distances
// EFFECTS : Places all possible city connections from currentCity into cities,
//           and all corresponding road distances into distances, with
//           corresponding entries at the same index.
void cityConnections(std::vector<char> &cities, std::vector<int> &distances,
                     const char &currentCity) {
    // Data based on map on page 95 of Russell, Norvig "Artificial Intelligence:
    // A Modern Approach"
    
    // Arad
    if (currentCity == 'A') {
        cities.push_back('S');
        distances.push_back(140);
        
        cities.push_back('T');
        distances.push_back(118);
        
        cities.push_back('Z');
        distances.push_back(75);
        
        return;
    }
    
    // Bucharest
    if (currentCity == 'B') {
        cities.push_back('F');
        distances.push_back(211);
        
        cities.push_back('G');
        distances.push_back(90);
        
        cities.push_back('P');
        distances.push_back(101);
        
        cities.push_back('U');
        distances.push_back(85);
        
        return;
    }
    
    // Craiova
    if (currentCity == 'C') {
        cities.push_back('D');
        distances.push_back(120);
        
        cities.push_back('P');
        distances.push_back(138);
        
        cities.push_back('R');
        distances.push_back(146);
        
        return;
    }
    
    // Dobreta
    if (currentCity == 'D') {
        cities.push_back('C');
        distances.push_back(120);
        
        cities.push_back('M');
        distances.push_back(75);
        
        return;
    }
    
    // Eforie
    if (currentCity == 'E') {
        cities.push_back('H');
        distances.push_back(86);
        
        return;
    }
    
    // Fagaras
    if (currentCity == 'F') {
        cities.push_back('B');
        distances.push_back(211);
        
        cities.push_back('S');
        distances.push_back(99);
        
        return;
    }
    
    // Giurgiu
    if (currentCity == 'G') {
        cities.push_back('B');
        distances.push_back(90);
        
        return;
    }
    
    // Hirsova
    if (currentCity == 'H') {
        cities.push_back('E');
        distances.push_back(86);
        
        cities.push_back('U');
        distances.push_back(98);
        
        return;
    }
    
    // Iasi
    if (currentCity == 'I') {
        cities.push_back('N');
        distances.push_back(87);
        
        cities.push_back('V');
        distances.push_back(92);
        
        return;
    }
    
    // Lugoj
    if (currentCity == 'L') {
        cities.push_back('M');
        distances.push_back(70);
        
        cities.push_back('T');
        distances.push_back(111);
        
        return;
    }
    
    // Mehadia
    if (currentCity == 'M') {
        cities.push_back('D');
        distances.push_back(75);
        
        cities.push_back('L');
        distances.push_back(70);
        
        return;
    }
    
    // Neamt
    if (currentCity == 'N') {
        cities.push_back('I');
        distances.push_back(87);
        
        return;
    }
    
    // Oradea
    if (currentCity == 'O') {
        cities.push_back('S');
        distances.push_back(151);
        
        cities.push_back('Z');
        distances.push_back(71);
        
        return;
    }
    
    // Pitesti
    if (currentCity == 'P') {
        cities.push_back('B');
        distances.push_back(101);
        
        cities.push_back('C');
        distances.push_back(138);
        
        cities.push_back('R');
        distances.push_back(97);
        
        return;
    }
    
    // Rimnicu Vilcea
    if (currentCity == 'R') {
        cities.push_back('C');
        distances.push_back(146);
        
        cities.push_back('P');
        distances.push_back(97);
        
        cities.push_back('S');
        distances.push_back(80);
        
        return;
    }
    
    // Sibiu
    if (currentCity == 'S') {
        cities.push_back('A');
        distances.push_back(140);
        
        cities.push_back('F');
        distances.push_back(99);
        
        cities.push_back('O');
        distances.push_back(151);
        
        cities.push_back('R');
        distances.push_back(80);
        
        return;
    }
    
    // Timisoara
    if (currentCity == 'T') {
        cities.push_back('A');
        distances.push_back(118);
        
        cities.push_back('L');
        distances.push_back(111);
        
        return;
    }
    
    // Urziceni
    if (currentCity == 'U') {
        cities.push_back('B');
        distances.push_back(85);
        
        cities.push_back('H');
        distances.push_back(98);
        
        cities.push_back('V');
        distances.push_back(142);
        
        return;
    }
    
    // Vaslui
    if (currentCity == 'V') {
        cities.push_back('I');
        distances.push_back(92);
        
        cities.push_back('U');
        distances.push_back(142);
        
        return;
    }
    
    // Zerind
    if (currentCity == 'Z') {
        cities.push_back('A');
        distances.push_back(75);
        
        cities.push_back('O');
        distances.push_back(71);
        
        return;
    }
    
    assert(false);
}

// REQUIRES: city is the first letter of a city in the problem.
// EFFECTS : returns the straight-line-distance between city and Bucharest
int straightLineDist(char city) {
    // Data based on map on page 95 of Russell, Norvig "Artificial Intelligence:
    // A Modern Approach"
    
    // Arad
    if (city == 'A') {
        return 366;
    }
    
    // Bucharest
    if (city == 'B') {
        return 0;
    }
    
    // Craiova
    if (city == 'C') {
        return 160;
    }
    
    // Dobreta
    if (city == 'D') {
        return 242;
    }
    
    // Eforie
    if (city == 'E') {
        return 161;
    }
    
    // Fagaras
    if (city == 'F') {
        return 178;
    }
    
    // Giurgiu
    if (city == 'G') {
        return 77;
    }
    
    // Hirsova
    if (city == 'H') {
        return 151;
    }
    
    // Iasi
    if (city == 'I') {
        return 226;
    }
    
    // Lugoj
    if (city == 'L') {
        return 244;
    }
    
    // Mehadia
    if (city == 'M') {
        return 241;
    }
    
    // Neamt
    if (city == 'N') {
        return 234;
    }
    
    // Oradea
    if (city == 'O') {
        return 380;
    }
    
    // Pitesti
    if (city == 'P') {
        return 98;
    }
    
    // Rimnicu Vilcea
    if (city == 'R') {
        return 193;
    }
    
    // Sibiu
    if (city == 'S') {
        return 253;
    }
    
    // Timisoara
    if (city == 'T') {
        return 329;
    }
    
    // Urziceni
    if (city == 'U') {
        return 80;
    }
    
    // Vaslui
    if (city == 'V') {
        return 199;
    }
    
    // Zerind
    if (city == 'Z') {
        return 374;
    }
    
    assert(false);
}
