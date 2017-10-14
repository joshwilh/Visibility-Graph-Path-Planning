//
//  main.cpp
//  A-Star Search
//
//  Created by Joshua Wilhelm on 10/3/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Tree_Node.h"
#include "search_functions.h"

using namespace std;

// Prints the solution vector (or failure) to cout
void printSoln(vector<char> solution) {
    if (solution.empty()) {
        cout << "Failure" << endl;
    }
    else {
        cout << "Success!" << endl;
        // Loop backwards through solution vector
        for (int i = (int) solution.size() - 1; i >= 0; --i) {
            cout << solution.at(i) << " ";
        }
        cout << endl;
    }
}

int main() {
    
    // Testing A* Search
    vector<char> solution;
    int distance;
    AStarSearch('A', solution, distance); // Start at Arad
    printSoln(solution); // Should return this path: A S R P B
    cout << "Distance Traveled: " << distance << " km" << endl; // Should be 418
}
