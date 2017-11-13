//
//  main.cpp
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 10/14/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include "List.h"
#include "preprocessing.h"
#include "AStarProblem.h"
#include "search_functions.h"

using namespace std;

// Initializes start and goal using user input
void Vertex_input (Vertex &start, Vertex &goal) {
    assert(DIMENSIONS == 2);
    cout << "Enter a starting point (format: \"x y\")" << endl;
    double x_in, y_in;
    cin >> x_in >> y_in;
    cout << endl;
    start.coord[0] = x_in;
    start.coord[1] = y_in;
    start.polygon = -1;
    start.location = -1;
    start.poly_size = -1;
    cout << "Enter a goal point (format: \"x y\")" << endl;
    cin >> x_in >> y_in;
    cout << endl;
    goal.coord[0] = x_in;
    goal.coord[1] = y_in;
    goal.polygon = -1;
    goal.location = -1;
    goal.poly_size = -1;
}

int main(int argc, const char * argv[]) {
    
    // Take in polygon file
    cout << "Enter a filename (polygon obstacles): ";
    string polygon_filename;
    cin >> polygon_filename;
    cout << endl;
    
    // Check if polygon file opens properly
    ifstream polygon_fin;
    polygon_fin.open(polygon_filename);
    
    if(!polygon_fin){
        cout << "Error opening file: " << polygon_filename << endl;
        return 1;
    }
    
    // Testing Preprocessing (see README)
    Graph g;
    // Hold the polygons List in main for checking the start and goal
    List<List<Vertex>> polygons(true);
    preProcess(g, polygon_fin, polygons);
    
    polygon_fin.close();
    
    cout << g << endl;
    
    Vertex *start = new Vertex;
    Vertex *goal = new Vertex;
    // Take start and goal input for A*
    Vertex_input(*start, *goal);
    
    // Create start and goal states
    State startState = {start};
    State goalState = {goal};
    
    // Create a problem
    Problem prob(g, startState, goalState);
    
    // Create a solution vector
    vector<State> solution;
    
    // Check visibility between start and goal
    if (visible(*start, *goal, polygons)) {
        // Create and print the solution
        cout << "Distance: " << distanceFormula(*start, *goal, DIMENSIONS)
             << endl;
        solution.push_back(goalState);
        solution.push_back(startState);
        prob.printSolution(solution, cout);
        
        delete start;
        delete goal;
        return 0;
    }
    
    // Add start and goal to graph (graph now owns them)
    g.vertices.insertStart(goal);
    g.vertices.insertStart(start);
    // Run visibleVertices on start and goal (checks all other vertices)
    List<Vertex>::Iterator it_start = g.vertices.begin();
    List<Vertex>::Iterator it_goal = it_start;
    ++it_goal;
    visibleVertices(it_start, g, polygons);
    visibleVertices(it_goal, g, polygons);
    // Now, start and goal should be properly inserted in the graph
    
    // Perform an A* search
    double distance = AStarSearch(prob, solution);
    // Print solution
    cout << "Distance: " << distance << endl;
    prob.printSolution(solution, cout);
    
    // Testing List
//    List<int> testList;
//    int* x = new int(6);
//    testList.insertStart(new int(5));
//    testList.insertEnd(x);
//    testList.insertEnd(new int(7));
//    testList.insertEnd(new int(8));
//    int* y = testList.nextItem(x);
//    assert(*y == 7);
//    y = testList.nextItem(y);
//    y = testList.nextItem(y);
//    assert(y == nullptr);
//    delete testList.removeStart();
//    delete testList.removeStart();
//    delete testList.removeStart();
//    delete testList.removeStart();
//    cout << "List tests passed!" << endl;
    //cout << testList.removeEnd() << endl;
    //testList.insertByValue(&x); // Should cause compiler error
     
    return 0;
}
