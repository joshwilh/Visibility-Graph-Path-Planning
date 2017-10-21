//
//  preprocessing.h
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 10/14/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#ifndef preprocessing_h
#define preprocessing_h

#include <vector>
#include "List.h"

// Forward declaration
struct Edge;

// Contains coordinates and list of edges
struct Vertex {
    double xPos;
    double yPos;
    List<Edge> connections;
};

// Contains two vertex endpoints and length of edge between them
struct Edge {
    Vertex v1;
    Vertex v2;
    double length;
};

// Contains list of vertices
struct Graph {
    List<Vertex> vertices;
};

//TODO
void preProcess(Graph &graph,
                std::vector<std::vector<double> > const &polygons);

// REQUIRES: graph is an empty Graph, polygons contains vectors of polygon
//           coordinates in the correct format.
// MODIFIES: graph
// EFFECTS : all of the vertices in polygons are added to graph with empty
//           'connections' lists
void addVertices(Graph &graph,
                 std::vector<std::vector<double> > const &polygons);

//TODO
// REQUIRES: vertices have been added to graph using addVertices. polygons
//           contains vectors of polygon coordinates in the correct format.
// MODIFIES: graph
// EFFECTS : Checks each vertex in graph for all visible vertices, and adds all
//           visible pairs as edges in graph
void makeConnections(Graph &graph, std::vector<std::vector<double> > &polygons);

//TODO
// REQUIRES: v is a vertex in a graph created from polygons. vertices have been
//           added to graph usingaddVertices. polygons contains vectors of
//           polygon coordinates in the correct format. v is not in the interior
//           of a polygon
// MODIFIES: *v
// EFFECTS : adds all visible vertices from v that are indexed higher (listed
//           later) in graph to v's connections as edges
void visibleVertices(Vertex *v, Graph &graph,
                     std::vector<std::vector<double> > const &polygons);

//TODO
// REQUIRES: v and check are valid vertices; v != check;
//           v and check are not in the interior of a polgon
// EFFECTS : returns true if check is visible from v (the line segment
//           connecting check and v does intersect any polygon edges);
//           returns false otherwise
bool visible(Vertex const v, Vertex const check,
             std::vector<std::vector<double> > const &polygons);

// REQUIRES: all parameters are valid vertices.
// EFFECTS : returns true if the line segment [a1, a2] intesects line segment
//           [b1, b2]
//           Credit: http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
//           Note: All general case behavior from website remains the same, with
//                 Example 2 remaining the same due to special cases 1) looking
//                 through polygon by way of two in-line vertices, or 2) looking
//                 through vertex of polygon - just move to that corner first.
//                 Special Case Example 1 returns false because flying along
//                 edges is allowed. Example 2 remains the same.
bool intersect(Vertex const a1, Vertex const a2,
               Vertex const b1, Vertex const b2);

#endif /* preprocessing_h */
