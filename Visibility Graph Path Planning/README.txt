README for Visibility Graph Path Planning

List

This is a singly-linked list code that can hold any type of data. Note that it uses templates, so List must be used like List<T>, where T is the type of data to store in list.
List is specialized to allow an insert-by-fCost for the Tree_Node type. This is done using the ‘id’ struct, as well as the private insertByValue function that is called from the public one. A compile-time error will occur if this function is called on a List that does not contain Tree_Node data. In the future, the insertByValue function will likely utilize the operator< to replace this behavior.
Note: DO NOT INCLUDE "List.cpp" in the target membership, as it is #included at the end of "List.h"

main

The main driver for the program. Requests the input file that contains the list of polygon obstacle coordinates from the user. Calls all functions related to the visibility graph path planning program.

Tree

A search tree container that holds Tree_Nodes. See the Tree_Node struct for the appropriate data members for each node. The tree must be given a root node in the constructor. The destructor recursively deletes all nodes in the tree.

search_functions

TODO

preprocessing

Preprocessing contains functions and data structures to create an initial visibility graph of a set of polygon obstacles. The function preProcess acts as the main interface, and requires an empty graph and an input stream of polygons. This function then calls other functions withing preprocessing to add the appropriates vertices and edges to the graph. After this process completes, the program is ready to receive a start and end point for the path planning problem.

polygon obstacle file format

Obstacle files must contain convex polygons represented as the (x,y) coordinates of vertices, preceded by the total number of vertices in the polygon. The vertices must be in CW or CCW orientation about the polygon. For example, two squares could be represented as:
"4\n0 0 0 1 1 1 1 0\n4\n 2 2 2 3 3 3 3 2"
