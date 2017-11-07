README for Visibility Graph Path Planning

List

This is a singly-linked list code that can hold any type of data. Note that it uses templates, so List must be used like List<T>, where T is the type of data to store in list.
List's insertByValue requires that the type T has an operator overload for <.
Note: DO NOT INCLUDE "List.cpp" in the target membership, as it is #included at the end of "List.h"

main

The main driver for the program. Requests the input file that contains the list of polygon obstacle coordinates from the user. Calls all functions related to the visibility graph path planning program.

Tree

A search tree container that holds Tree_Nodes. See the Tree_Node struct for the appropriate data members for each node. The tree must be given a root node in the constructor. The destructor recursively deletes all nodes in the tree.

search_functions

These are the domain-independent parts of the A-star search. These functions assume that a Problem class and a State struct will be provided with the appropriate functions and operator overloads. Calling AStarSearch on a Problem object will return the solution path and the total distance traveled according to an A* search algorithm.

AStarProblem and AStarState

These are the domain-dependent parts of the A-star search. For A* to work properly, all functions in the interfaces "AStarProblem.h" and "AStarState.h" must be implemented. Note that while the function names, return types, and parameter lists should not change in general, the member variables of Problem (and therefore also the constructor) may need to change for a given problem.

preprocessing

Preprocessing contains functions and data structures to create an initial visibility graph of a set of polygon obstacles. The function preProcess acts as the main interface, and requires an empty graph and an input stream of polygons. This function then calls other functions withing preprocessing to add the appropriates vertices and edges to the graph. After this process completes, the program is ready to receive a start and end point for the path planning problem.

polygon obstacle file format

Obstacle files must contain convex polygons represented as the (x,y) coordinates of vertices, preceded by the total number of vertices in the polygon. The vertices must be in CW or CCW orientation about the polygon. For example, two squares could be represented as:
"4\n0 0 0 1 1 1 1 0\n4\n 2 2 2 3 3 3 3 2"
