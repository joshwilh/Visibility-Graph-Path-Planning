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
#include <iostream>
#include "List.h"

const int DIMENSIONS = 2;

// Contains coordinates
struct Vertex {
    double coord[DIMENSIONS];
};

// Contains a linked list of coordinates
struct Polygon {
    List<Vertex> polygonVertices;
};

// Contains two vertex endpoints and length of edge between them
struct Edge {
    const Vertex* v1;
    const Vertex* v2;
    double length;
};

// Contains list of vertices and paths between the vertices
struct Graph {
    List<Vertex> vertices;
    List<Edge> connections;
};

// REQUIRES: graph is an empty graph, polygonFile has been opened properly and
//           contains polygons in the proper format, polygons cannot overlap,
//           nor can they share vertices, edges, or have a vertex on the edge of
//           another polygon
// MODIFIES: graph, polygonFile
// EFFECTS : reads polygonFile, then calls addVertices, then makeConnections
//           (which calls visibleVertices on each vertex)
void preProcess(Graph &graph, std::istream& polygonFile);

// REQUIRES: graph is an empty Graph, polygons contains polygon objects with
//           coordinates in the correct format
// MODIFIES: graph
// EFFECTS : all of the vertices in polygons are added to graph
void addVertices(Graph &graph, List<Polygon> const &polygons);

// REQUIRES: graph has been successfully passed through addVertices.
//           polygons contains valid polygon obstacles
// MODIFIES: graph
// EFFECTS : Checks each vertex in graph for all visible vertices
void makeConnections(Graph &graph, List<Polygon> const &polygons);

// REQUIRES: v is a vertex in graph. graph has been successfully passed through
//           addVertices, v is not in the interior of a polygon,
//           0 <= index < graph.vertices.size()
// MODIFIES: graph
// EFFECTS : adds all possible paths from v that are indexed higher (listed
//           later) in graph to graph as edges
void visibleVertices(const Vertex *v, Graph &graph, const int index,
                     List<Polygon> const &polygons);

// REQUIRES: v and check are valid vertices; v != check;
//           v and check are not in the interior of a polgon
//           polygons contains valid polygon objects
// EFFECTS : returns true if check is visible from v (the line segment
//           connecting check and v does intersect any polygon edges);
//           returns false otherwise
bool visible(const Vertex& v, const Vertex& check,
             List<Polygon> const &polygons);

// REQUIRES: all parameters are valid vertices.
// EFFECTS : returns true if the line segment [a1, a2] intesects line segment
//           [b1, b2]
//           Credit: http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
//           Note: All general case behavior from website remains the same, with
//                 Example 2 remaining the same due to special cases 1) looking
//                 through polygon by way of two in-line vertices, or 2) looking
//                 through vertex of polygon - just move to that corner first.
//                 HOWEVER, Example 2 returns false in the special case that the
//                 two line segments share a common endpoint.
//                 Special Case Example 1 returns false because flying along
//                 edges is allowed. Example 2 remains the same.
bool intersect(Vertex const& a1, Vertex const& a2,
               Vertex const& b1, Vertex const& b2);

// EFFECTS : Returns true if lhs and rhs have the same coordinates.
bool operator==(const Vertex &lhs, const Vertex &rhs);

#endif /* preprocessing_h */
