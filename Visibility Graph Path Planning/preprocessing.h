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
    // Which polygon the Vertex is a member of (-1 if not part of a polygon)
    int polygon;
    // Which index within that polygon Vertex is (-1 if not part of a polygon)
    int location;
    // How many vertices the polygon that Vertex is part of contains (-1 if not
    // part of a polygon)
    int poly_size;
    
    // Vertex assignment operator
    Vertex& operator=(const Vertex& rhs);
};

// Contains a linked list of coordinates
//struct Polygon {
//    List<Vertex> polygonVertices;
//};

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
    
    // Constructs an empty graph that owns its vertices and edges
    Graph();
};

// REQUIRES: dimensions > 0, v1.coord[] and v2.coord[] are of size dimensions
// EFFECTS : returns the distance between v1 and v2
double distanceFormula(const Vertex& v1, const Vertex& v2,
                       const int dimensions);

// REQUIRES: graph is an empty graph, polygonFile has been opened properly and
//           contains polygons in the proper format, polygons cannot overlap,
//           nor can they share vertices, edges, or have a vertex on the edge of
//           another polygon
//           polygons is empty
// MODIFIES: graph, polygonFile, polygons
// EFFECTS : reads polygonFile, then calls addVertices, then makeConnections
//           (which calls visibleVertices on each vertex). polygons can later be
//           used to call specific functions in preprocessing. Adds the loops
//           run by each preprocessing function to the appropriate loop counter.
void preProcess(Graph &graph, std::istream& polygonFile,
                List<List<Vertex>> &polygons, int &readPolygonsCounter,
                int &addVerticesCounter, int &makeConnectionsCounter,
                int &visibleVerticesCounter, int &visibleCounter);

// REQUIRES: graph is an empty Graph, polygons contains polygon objects with
//           coordinates in the correct format
// MODIFIES: graph
// EFFECTS : all of the vertices in polygons are added to graph. Adds the number
//           of loops run to loopCounter
void addVertices(Graph &graph, List<List<Vertex>> const &polygons,
                 int &loopCounter);

// REQUIRES: graph has been successfully passed through addVertices.
//           polygons contains valid polygon obstacles
// MODIFIES: graph
// EFFECTS : Checks each vertex in graph for all visible vertices. Adds the
//           number of loops performed to loopCounter.
void makeConnections(Graph &graph, List<List<Vertex>> const &polygons,
                     int &loopCounter, int &visibleVerticesCounter,
                     int &visibleCounter);

// REQUIRES: v is an Iterator that points to a vertex in graph. graph has been
//           successfully passed through addVertices, v's vertex is not in the
//           interior of a polygon.
// MODIFIES: graph
// EFFECTS : adds all possible paths from v that are indexed higher (listed
//           later) in graph to graph as edges. Adds the number of loops run to
//           loopCounter
void visibleVertices(List<Vertex>::Iterator v, Graph &graph,
                     List<List<Vertex>> const &polygons, int &loopCounter,
                     int &visibleCounter);

// NOTE: This overload is for use when finding the edges added by start and goal
//       in the visibility graph.
// REQUIRES: v is an Iterator that points to a vertex in graph. graph has been
//           successfully passed through addVertices, v's vertex is not in the
//           interior of a polygon.
// MODIFIES: graph
// EFFECTS : adds all possible paths from v that are indexed higher (listed
//           later) in graph to graph as edges. Also adds these edges to victims
//           for later deletion. Adds the number of loops run to loopCounter.
void visibleVertices(List<Vertex>::Iterator v, Graph &graph,
                     List<List<Vertex>> const &polygons, int &loopCounter,
                     int &visibleCounter, List<Edge> &victims);

// REQUIRES: v and check are valid vertices; v != check;
//           v and check are not in the interior of a polgon
//           polygons contains valid polygon objects
// EFFECTS : returns true if check is visible from v (the line segment
//           connecting check and v does intersect any polygon edges);
//           returns false otherwise. Adds the number of loops run to
//           loopCounter
bool visible(const Vertex& v, const Vertex& check,
             List<List<Vertex>> const &polygons, int &loopCounter);

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

// REQUIRES: v is a valid Vertex
// MODIFIES: os
// EFFECTS : Prints the Vertex to os
std::ostream & operator<<(std::ostream &os, const Vertex &v);

// REQUIRES: e is a valid Edge
// MODIFIES: os
// EFFECTS : Prints the Edge to os (Vertices only)
std::ostream & operator<<(std::ostream &os, const Edge &e);

// REQUIRES: g is a valid Graph
// MODIFIES: os
// EFFECTS : Prints the vertices and connections lists to os
std::ostream & operator<<(std::ostream &os, const Graph &g);

#endif /* preprocessing_h */
