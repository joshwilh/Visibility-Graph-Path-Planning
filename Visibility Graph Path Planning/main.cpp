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
#include <math.h>
#include "List.h"
#include "preprocessing.h"
#include "AStarProblem.h"
#include "search_functions.h"

using namespace std;

const double PI = 3.14159265358979323846;
// Parameters for testing
static const int MAXPOLYGONS = 100;
static const int MAXVERTICES = 20;
static const int NUMOFSEARCHES = 20;

// Initializes start and goal using user input
void Vertex_input (Vertex &start, Vertex &goal);

// EFFECTS: Generates a random double between fMin and fMax
double fRand(double fMin, double fMax);

// REQUIRES: arr points to the first element of a valid array of size doubles
// MODFIEIS: arr
// EFFECTS : Sorts arr from smallest to largest
void insertionSort(double *arr, int size);

// MODIFIES: a, b
// EFFECTS : Swaps the values of a and b
void swap (double &a, double &b);

// REQUIRES: testIndex >= 0 and testIndex < numOfTests, polygonFile and
//           outputFile are open
// EFFECTS : Produces a visibility graph and runs AStarSearches on the graph.
//           Outputs the results to output[testIndex].txt
void runTest(int testIndex, ostream &polygonFile, ostream &outputFile);

int main(int argc, const char * argv[]) {
    
    ////////////// Begin testing code //////////////////////
    
//    // Total runtime in seconds
//    time_t programBegin = time(nullptr);
//
//    // Total number of visibility graphs that will be produced
//    const int numOfTests = 1000;
//
//    // Seed random number generation
//    srand((int) time(nullptr));
//
//    // Create polygons and output files
//    ofstream polygonFile("output_polygons.out");
//    ofstream outputFile("output.out");
//
//    // Each test
//    for (int testIndex = 0; testIndex < numOfTests; ++testIndex) {
//        runTest(testIndex, polygonFile, outputFile);
//    }
//
//    // Output total testing time and testing parameters
//    outputFile << "Total_Time: " << time(nullptr) - programBegin << endl;
//    outputFile << "Testing_Parameters: MAXPOLYGONS: " << MAXPOLYGONS
//               << " MAXVERTICES: " << MAXVERTICES << " NUMOFSEARCHES: "
//               << NUMOFSEARCHES << endl;
//
//    polygonFile.close();
//    outputFile.close();
    
    ////////////// End testing code //////////////////////
    
    ////////////// Begin single run code //////////////////////
    
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

    // Need dummy counters (see testing code for actual use)
    int dummyCounter = 0;
    preProcess(g, polygon_fin, polygons, dummyCounter, dummyCounter,
               dummyCounter, dummyCounter, dummyCounter);

    polygon_fin.close();

    // cout << g << endl;

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
    if (visible(*start, *goal, polygons, dummyCounter)) {
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
    visibleVertices(it_start, g, polygons, dummyCounter, dummyCounter);
    visibleVertices(it_goal, g, polygons, dummyCounter, dummyCounter);
    // Now, start and goal should be properly inserted in the graph

    // Perform an A* search
    size_t dummySize = 0;
    double distance = AStarSearch(prob, solution, dummyCounter, dummyCounter,
                                  dummySize);
    // Print solution
    cout << "Distance: " << distance << endl;
    prob.printSolution(solution, cout);
    
    ////////////// End single run code //////////////////////
     
    return 0;
}

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

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void insertionSort(double *arr, int size) {
    for (int i = 1; i < size; ++i) {
        for (int j = i; j > 0; --j) {
            if (arr[j - 1] > arr[j]) {
                break;
            }
            swap(arr[j], arr[j - 1]);
        }
    }
}

void swap (double &a, double &b) {
    double temp = a;
    a = b;
    b = temp;
}

void runTest(int testIndex, ostream &polygonFile, ostream &outputFile) {
    
    clock_t generationStart = clock(); // Time in clock ticks
    
    // Output test index to both output files
    polygonFile << "test_";
    outputFile << "test_";
    if (testIndex < 10) {
        polygonFile << "000" << testIndex << endl << endl;
        outputFile << "000" << testIndex << endl << endl;
    } else if (testIndex < 100) {
        polygonFile << "00" << testIndex << endl << endl;
        outputFile << "00" << testIndex << endl << endl;
    } else if (testIndex < 1000) {
        polygonFile << "0" << testIndex << endl << endl;
        outputFile << "0" << testIndex << endl << endl;
    } else {
        polygonFile << testIndex << endl << endl;
        outputFile << testIndex << endl << endl;
    }
    
    // Define plane for search as square from (-100, -100) at bottom left
    // to (100, 100) at top right
    
    //random int in interval [1,MAXPOLYGONS]
    const int numOfPolygons = (rand() % MAXPOLYGONS) + 1;
    int totalVertices = 0;
    
    // Store center coordinates and radii of all circles to avoid overlap
    Vertex *arrCenters = new Vertex[numOfPolygons];
    double *arrRadii = new double[numOfPolygons];
    
    // Create single polygons file for this test
    // Set high precision to avoid floating-point errors during testing
    ofstream localPolygonFile("polygons.txt");
    localPolygonFile.precision(20);
    
    // Each polygon to be generated
    for (int polygonIndex = 0; polygonIndex < numOfPolygons; ++polygonIndex) {
        
        bool polygonPlaced = false;
        while (!polygonPlaced) {
            // Set a random center point
            Vertex center = {fRand(-100, 100), fRand(-100, 100), -1, -1, -1};
            
            // Find upper bound on radius
            // First, base upper bound on position of center
            double upperBoundRadius = min(100 - fabs(center.coord[0]),
                                          100 - fabs(center.coord[1]));
            // Then, update upper bound based on other polygons
            for (int i = 0; i < polygonIndex; ++i) {
                double maxLocalRadius =
                    distanceFormula(center, arrCenters[i], DIMENSIONS)
                    - arrRadii[i];
                
                if (maxLocalRadius < upperBoundRadius) {
                    upperBoundRadius = maxLocalRadius;
                }
                if (upperBoundRadius <= 0) {
                    // Need to pick new center (current center is within an
                    // existing circle)
                    break;
                }
            }
            
            if (upperBoundRadius <= 0) { continue; }
            
            // Now, can be certain that center and radius are acceptable
            double radius = fRand(0, upperBoundRadius);
            
            // Store center and radius for future
            arrCenters[polygonIndex] = center;
            arrRadii[polygonIndex] = radius;
            
            //random int in [3,MAXVERTICES]
            int numOfVertices = (rand() % (MAXVERTICES - 2)) + 3;
            // Store an angle for each vertex to be placed
            double *angles = new double[numOfVertices];
            // Fill with random angles [0, 2*PI]
            for (int i = 0; i < numOfVertices; ++i) {
                angles[i] = fRand(0, 2 * PI);
            }
            // Sort angles into CCW order
            insertionSort(angles, numOfVertices);
            
            // Write to polygons file
            polygonFile << numOfVertices << endl;
            localPolygonFile << numOfVertices << endl;
            for (int i = 0; i < numOfVertices; ++i) {
                double xCoord = center.coord[0] + radius * cos(angles[i]);
                double yCoord = center.coord[1] + radius * sin(angles[i]);
                polygonFile << xCoord << " " << yCoord << " ";
                localPolygonFile << xCoord << " " << yCoord << " ";
            }
            polygonFile << endl;
            localPolygonFile << endl;
            
            totalVertices += numOfVertices;
            delete [] angles;
            polygonPlaced = true;
        }
    }
    
    // Ends output to polygon files
    localPolygonFile.close();
    polygonFile << endl;
    
    // number of polygons and number of vertices
    outputFile << numOfPolygons << " " << totalVertices << endl;
    
    // Generate NUMOFSEARCHES pairs of start and end points that do not lie
    // inside any polygon
    Vertex startPoints[NUMOFSEARCHES];
    Vertex endPoints[NUMOFSEARCHES];
    
    for (int i = 0; i < 2*NUMOFSEARCHES; ++i) {
        bool goodPoint = false;
        while (!goodPoint) {
            // Set a random point
            Vertex point = {fRand(-100, 100), fRand(-100, 100), -1, -1, -1};
            goodPoint = true;
            for (int j = 0; j < numOfPolygons; ++j) {
                // Check whether inside any polygon
                if (distanceFormula(arrCenters[j], point, DIMENSIONS)
                    <= arrRadii[j]) {
                    goodPoint = false;
                    break;
                }
            }
            if (goodPoint) {
                // Set as start or end
                if (i % 2 == 0) {
                    startPoints[i / 2] = point;
                } else {
                    endPoints[i / 2] = point;
                }
            }
        }
    }
    
    // output time for generation (clock ticks)
    outputFile << clock() - generationStart << endl << endl;
    
    // Circle info no longer needed
    delete [] arrCenters;
    arrCenters = nullptr;
    delete [] arrRadii;
    arrRadii = nullptr;
    
    // Check if polygon file opens properly
    ifstream polygon_fin("polygons.txt");
    if(!polygon_fin){
        cout << "Error opening file: polygons.txt" << endl;
        assert(false);
    }
    
    // Start the clock for preprocessing
    clock_t preprocessStart = clock(); // Time in clock ticks
    
    // This graph will hold the visibility graph made up of only polygon
    // vertices (not start and goal points), allowing graphs for each run to be
    // created from it.
    Graph ownerGraph;
    // Hold the polygons List in main for checking the start and goal
    List<List<Vertex>> polygons(true);
    // Create all loop counters for preprocessing
    int readPolygonsCounter = 0;
    int addVerticesCounter = 0;
    int makeConnectionsCounter = 0;
    int visibleVerticesCounter = 0;
    int visibleCounter = 0;
    
    // Create the visibility graph
    preProcess(ownerGraph, polygon_fin, polygons, readPolygonsCounter,
               addVerticesCounter, makeConnectionsCounter,
               visibleVerticesCounter, visibleCounter);
    
    polygon_fin.close();
    
    // Stop the clock for preprocessing
    clock_t preprocessTime = clock() - preprocessStart;
    
    // Output preprocess information to file
    outputFile << ownerGraph.vertices.size() << " "
               << ownerGraph.connections.size() << endl;
    outputFile << readPolygonsCounter << " " << addVerticesCounter << " "
               << makeConnectionsCounter << " " << visibleVerticesCounter << " "
               << visibleCounter << endl;
    outputFile << preprocessTime << endl << endl;
    
    // Run each Search
    for (int search = 0; search < NUMOFSEARCHES; ++search) {
        // Copy-construct a new graph (won't own data, but that's ok)
        Graph searchGraph(ownerGraph);
        // This list will ensure deletion of any newly-created edges for
        // searchGraph
        List<Edge> victims(true);
        
        Vertex *start = &startPoints[search];
        Vertex *goal = &endPoints[search];
        
        // Output coordinates to file
        outputFile << start->coord[0] << " " << start->coord[1] << " "
        << goal->coord[0] << " " << goal->coord[1] << endl;
        
        // Create start and goal states
        State startState = {start};
        State goalState = {goal};
        
        // Create a problem
        Problem prob(searchGraph, startState, goalState);
    
        // Create a solution vector
        vector<State> solution;
        
        // Check visibility between start and goal
        int dummyCounter = 0;
        int dummyCounter2 = 0;
        if (visible(*start, *goal, polygons, dummyCounter)) {
            // No need for A*, output everything now
            solution.push_back(goalState);
            solution.push_back(startState);
            prob.printSolution(solution, outputFile);
            outputFile << distanceFormula(*start, *goal, DIMENSIONS) << endl
                       << endl;
        } else {
            // Add start and goal to graph
            searchGraph.vertices.insertStart(goal);
            searchGraph.vertices.insertStart(start);
            // Run visibleVertices on start and goal (checks all other vertices)
            List<Vertex>::Iterator it_start = searchGraph.vertices.begin();
            List<Vertex>::Iterator it_goal = it_start;
            ++it_goal;
            visibleVertices(it_start, searchGraph, polygons, dummyCounter,
                            dummyCounter2, victims);
            visibleVertices(it_goal, searchGraph, polygons, dummyCounter,
                            dummyCounter2, victims);
            // Now, start and goal should be properly inserted in the graph
        
            // Start the clock for A*
            clock_t AStarStart = clock(); // Time in clock ticks
            
            // Perform an A* search
            int nodesExpandedCounter = 0;
            int maxTreeSize = 0;
            size_t memoryUse = 0;
            double distance = AStarSearch(prob, solution, nodesExpandedCounter,
                                          maxTreeSize, memoryUse);
            
            // Stop the clock for A*
            clock_t AStarTime = clock() - AStarStart;
            
            // Output to file
            prob.printSolution(solution, outputFile);
            outputFile << distance << endl;
            outputFile << searchGraph.vertices.size() << " "
                       << searchGraph.connections.size() << endl;
            outputFile << nodesExpandedCounter << " " << AStarTime << endl;
            outputFile << maxTreeSize << " " << memoryUse << endl << endl;
        }
    }
    
    // Print newline before next test
    outputFile << endl;
}
