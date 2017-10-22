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
// REQUIRES: graph is an empty graph, polygonFile has been opened properly and
//           contains polygons in the proper format, polygons cannot overlap,
//           nor can they share vertices, edges, or have a vertex on the edge of
//           another polygon
// MODIFIES: graph, polygonFile
// EFFECTS : reads polygonFile, then calls addVerticesAndEdges, then
//           makeConnections (which calls visibleVertices on each vertex)
void preProcess(Graph &graph, std::istream& polygonFile);

// REQUIRES: graph is an empty Graph, polygons contains vectors of polygon
//           coordinates in the correct format, polygonEdges is an empty vector.
// MODIFIES: graph, polygonEdges
// EFFECTS : all of the vertices in polygons are added to graph with empty
//           'connections' lists, fills polygonEdges with edges to be used in
//           the visible function
void addVerticesAndEdges(Graph &graph,
                         std::vector<std::vector<double> > const &polygons,
                         std::vector<Edge> &polygonEdges);

// REQUIRES: graph and polygonEdges have been successfully passed through
//           addVerticesAndEdges.
//           contains vectors of polygon coordinates in the correct format.
// MODIFIES: graph
// EFFECTS : Checks each vertex in graph for all visible vertices, and adds all
//           visible pairs as edges in graph
void makeConnections(Graph &graph, std::vector<Edge> const &polygonEdges);

// REQUIRES: v is a vertex in graph. graph and polygonEdges have been
//           successfully passed through addVerticesAndEdges, v is not in the
//           interior of a polygon
// MODIFIES: *v, graph
// EFFECTS : adds all visible vertices from v that are indexed higher (listed
//           later) in graph to v's connections as edges, and adds v to those
//           vertices' connections lists
void visibleVertices(Vertex *v, Graph &graph,
                     std::vector<Edge> const &polygonEdges);

// REQUIRES: v and check are valid vertices; v != check;
//           v and check are not in the interior of a polgon
//           polygonEdges has been properly constructed in generateEdges
// EFFECTS : returns true if check is visible from v (the line segment
//           connecting check and v does intersect any polygon edges);
//           returns false otherwise
bool visible(Vertex const v, Vertex const check,
             std::vector<Edge> const polygonEdges);

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
