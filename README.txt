README for Visibility Graph Path Planning

Instructions for Use


Single-Run Mode

1. Comment out the code between "Begin testing code" and "End testing code" in main.cpp. Uncomment the code between "Begin single run code" and "End single run code".

2. Compile the path planning code by using the "make pathplan.exe" command.

3. Create a polygon file in the correct format (see below).

4. Run the path planning program with the "./pathplan.exe" command. Follow the prompts by providing the polygon file name and start and goal points.

5. The program will output a solution path and distance to the console.


Testing Mode

1. Uncomment the code between "Begin testing code" and "End testing code" in main.cpp. Comment out the code between "Begin single run code" and "End single run code".

2. Set the testing parameters near the top of main.cpp. The default number of visibility graph tests is 1000. Change this number in line 57 of main.cpp if desired.

3. Compile the path planning code by using the "make pathplan.exe" command.

4. Run the path planning program with the "./pathplan.exe" command (or "nohup ./pathplan.exe &"). The program requires no input, and will write the testing results to "output.out". All of the generated polygon files are collected in "output_polygons.out".

5. Compile the post-processing code using "make postprocess.exe".

6. Run the post-processing code with the "./postprocess.exe" command. Provide the "output.out" filename from step 4 when prompted. This program outputs two files in a MATLAB-friendly format: "post_process_vis_graph.out" and "post_process_AStar.out".

7. Copy the two files from step 6 into the same folder as the MATLAB "Post_Process.m" file. Enter the visibility graph's filename in line 9 of the m-file, and the A*'s filename in line 101. Run the script to produce plots.


Polygon and Solution Path Visualization

1. Generate a polygon obstacle file in the format described below. This can be done either manually or by using one of the maps produced by the Testing Mode.

2. Generate a solution path using either the single-run mode or the testing mode. Save this solution path in a text file.

3. Save both test files in the same folder as "Draw_Polygons_and_Solutions.m". Enter the filename of the polygon file in line 4. Enter the filename of the solution path file in line 29.

4. Run the script, which will produce a single plot.

--------------------------------------------------------------------------------

List

This is a singly-linked list code that can hold any type of data. Note that it uses templates, so List must be used like List<T>, where T is the type of data to store in list.
List's insertByValue requires that the type T has an operator overload for <.
Note: DO NOT INCLUDE "List.tpp" in the target membership, as it is #included at the end of "List.h"

main

The main driver for the program. To use uncomment either the testing code or the single run code and comment out the other.
If the testing code is used: Runs the specified amount of tests, no user input is required.
If the single run code is used: Requests the input file that contains the list of polygon obstacle coordinates from the user. Calls all functions related to the visibility graph path planning program.

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

Testing Code: "output_polygons.out" file format

The output_polygons file contains the contents of every "polygons.txt" generated during testing. Each entry begins with "test_####", then a blank line, and is followed by contents identical to "polygons.txt". Another blank line is placed after each set of polygons, at which point the next entry begins.

Testing Code: "output.out" file format

The output file contains information regarding each test run by the testing code. Each entry begins with "test_####", then a blank line, followed by a line containing the number of polygons generated and the number of total vertices in all of those polygons. The next line contains the number of clock cycles required by the random polygon and start/end point generation. The next line contains the number of vertices and edges in the visibility graph, not including the start and end vertices. Following this line, the numbers of loops run by each of the preprocessing functions are printed. The next line holds the total number of clock cycles required for preprocessing. After this information, information about each A* search is printed. This begins with the coordinates of the start and end points. Then, the word "Failure" or "Success!" is printed. If the search was successful, the solution path and distance are printed. Then, the size of the visibility graph including the start and end vertices is printed, if A* was needed. This is followed by a line containing the number of nodes expanded by the A* search and the total time required by A*, if A* was needed. Then, another line follows with the maximum search tree size during A* and the memory used by the program just before A* returned. The final lines in the output file include the total number of seconds required by the program and the testing parameters.
Format:
test_[test number]\n
[number of polygons generated] [total number of vertices in all generated polygons]
[number of clock cycles required by the random polygon and start/end point generation]\n
[number of vertices in visibility graph (not including start/end)] [number of edges in visibility graph (not including start/end)]
[number of loops run by readPolygons] [number of loops run by addVertices] [number of loops run by makeConnections] [number of loops run by visibleVertices] [number of loops run by visible]
[number of clock cycles required for preprocessing]\n
[A* section (see below)]

Each A* section:
[x-coordinate of start point] [y-coordinate of start point] [x-coordinate of end point] [y-coordinate of end point]
[Failure or success]
[Solution path (if success)]
[Solution distance (if success) - this is the last line of this section if start and end were directly visible from each other]
[number of vertices in visibility graph (including start/end)] [number of edges in visibility graph (including start/end)]
[number of nodes expanded by A*] [number of clock cycles required for A*]
[maximum search tree size] [memory usage at end of A*]\n

postprocess

This code takes in an output file and processes it into a MATLAB-friendly format (rows and columns). The structure of the post-processed files are as follows:
Visibility Graph (pre-process) file consists of rows of these values, separated by spaces as shown:
[number of vertices in visibility graph] [number of edges in visibility graph] [readPolygon loop counter] [addVertices loop counter] [makeConnections loop counter] [visibleVertices loop counter] [visible loop counter] [clock cycles]\n
A* file consists of one special row containing the number of success and failures:
[number of successes] [number of failures] 0 0 0 0\n
Then, the file consists of rows of these values, separated by spaces as shown:
[number of vertices in visibility graph (including start and goal)] [number of edges] [number of nodes expanded] [clock cycles] [maximum search tree size] [memory usage at end of A*]\n
