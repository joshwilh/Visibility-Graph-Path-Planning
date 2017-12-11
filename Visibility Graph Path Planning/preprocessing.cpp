//
//  preprocessing.cpp
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 10/16/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <stdio.h>
#include <cmath>
#include "preprocessing.h"

using namespace std;

// Constructs an empty graph that owns its vertices and edges
Graph::Graph() :
vertices(true), connections(true) {}

// REQUIRES: a and b do not have extremely large absolute values
// EFFECTS: returns true if a and b are within EPSILON of one another,
//         false otherwise
static bool compareDoubles(double a, double b) {
    return fabs(a - b) < __DBL_EPSILON__;
}

// REQUIRES: dimensions > 0, v1.coord[] and v2.coord[] are of size dimensions
// EFFECTS : returns the distance between v1 and v2
double distanceFormula(const Vertex& v1, const Vertex& v2,
                              const int dimensions) {
    double sumOfSquares = 0;
    for (int i = 0; i < dimensions; ++i) {
        sumOfSquares += pow(v1.coord[i] - v2.coord[i], 2);
    }
    
    return sqrt(sumOfSquares);
}


// REQUIRES: all parameters are valid vertices
// EFFECTS : returns 1 if orientation of (p1, p2, p3) is clockwise
//           returns -1 if orientation of (p1, p2, p3) is counterclockwise
//           returns 0 if (p1, p2, p3) are colinear
// NOTE    : Helper function for intersect
//           Credit: http://www.geeksforgeeks.org/orientation-3-ordered-points/
static int orientation (Vertex const p1, Vertex const p2, Vertex const p3) {
    // Orientation formula: (y2 - y1)*(x3 - x2) - (y3 - y2)*(x2 - x1)
    double result = (p2.coord[1] - p1.coord[1]) * (p3.coord[0] - p2.coord[0])
                 - (p3.coord[1] - p2.coord[1]) * (p2.coord[0] - p1.coord[0]);
    if (compareDoubles(result, 0.0)) {
        return 0;
    }
    if (result > 0) {
        return 1;
    }
    return -1;
}

// REQUIRES: polygons is an empty List of polygons, 'is' is a valid
//           input stream that has been opened and is in the correct format
// MODIFIES: polygons
// EFFECTS : reads vertices from is and places them into polygons. Adds the
//           number of loops run to loop counter
static void read_polygons(List<List<Vertex>>& polygons, istream& is,
                          int &loopCounter) {
    assert(polygons.empty());
    
    int polygon_index = 0;
    int num_vertices;
    
    // Reads one polygon per loop
    while (is >> num_vertices) {
        
        // Reads all vertex coordinates for this polygon
        List<Vertex> *polygon = new List<Vertex>(true);
        for (int i = 0; i < DIMENSIONS * num_vertices; i += DIMENSIONS) {
            // DIMENSIONS = num of coord per vertex
            Vertex *v = new Vertex;
            for (int j = 0; j < DIMENSIONS; ++j) {
                is >> v->coord[j];
            }
            v->polygon = polygon_index;
            v->location = i / DIMENSIONS;
            v->poly_size = num_vertices;
            polygon->insertEnd(v);
            
            ++loopCounter;
        }
        
        polygons.insertEnd(polygon);
        
        ++polygon_index;
    }
}

// Vertex assignment operator
Vertex& Vertex::operator=(const Vertex& rhs) {
    for (int i = 0; i < DIMENSIONS; ++i) {
        coord[i] = rhs.coord[i];
    }
    polygon = rhs.polygon;
    location = rhs.location;
    poly_size = rhs.poly_size;
    
    return *this;
}

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
                int &visibleVerticesCounter, int &visibleCounter) {
    
    // polygons will own the dynamically allocated polygons
    read_polygons(polygons, polygonFile, readPolygonsCounter);
    
    addVertices(graph, polygons, addVerticesCounter);
    
    makeConnections(graph, polygons, makeConnectionsCounter,
                    visibleVerticesCounter, visibleCounter);
}

// REQUIRES: graph is an empty Graph, polygons contains polygon objects with
//           coordinates in the correct format
// MODIFIES: graph
// EFFECTS : all of the vertices in polygons are added to graph. Adds the number
//           of loops run to loopCounter
void addVertices(Graph &graph, List<List<Vertex>> const &polygons,
                 int &loopCounter) {
    // Check graph is empty
    assert(graph.vertices.empty() && graph.connections.empty());
    
    // Traversal by Iterator
    List<List<Vertex>>::Iterator endP = polygons.end();
    for (List<List<Vertex>>::Iterator currentPolygon = polygons.begin();
         currentPolygon != endP; ++currentPolygon) {
        
        // Can't have a polygon with fewer than 3 vertices
        assert((*currentPolygon)->size() >= 3);
        
        // Traversal by Iterator
        List<Vertex>::Iterator endV = (*currentPolygon)->end();
        for (List<Vertex>::Iterator currentVertex = (*currentPolygon)->begin();
             currentVertex != endV; ++currentVertex) {
            // Each vertex in this polygon
            
            Vertex* newVert = new Vertex;
            *newVert = **currentVertex;
            graph.vertices.insertEnd(newVert);
            
            ++loopCounter;
        }
    }
}

// REQUIRES: graph has been successfully passed through addVertices.
//           polygons contains valid polygon obstacles
// MODIFIES: graph
// EFFECTS : Checks each vertex in graph for all visible vertices. Adds the
//           number of loops performed to loopCounter.
void makeConnections(Graph &graph, List<List<Vertex>> const &polygons,
                     int &loopCounter, int &visibleVerticesCounter,
                     int &visibleCounter) {
    
    // If graph is empty, no connections to be made
    if (graph.vertices.empty()) {
        return;
    }
    
    // Traversal by Iterator
    List<Vertex>::Iterator end = graph.vertices.end();
    for (List<Vertex>::Iterator v = graph.vertices.begin(); v != end; ++v) {
        visibleVertices(v, graph, polygons, visibleVerticesCounter,
                        visibleCounter);
        
        ++loopCounter;
    }
}

// REQUIRES: v is an Iterator that points to a vertex in graph. graph has been
//           successfully passed through addVertices, v's vertex is not in the
//           interior of a polygon.
// MODIFIES: graph
// EFFECTS : adds all possible paths from v that are indexed higher (listed
//           later) in graph to graph as edges. Adds the number of loops run to
//           loopCounter
void visibleVertices(List<Vertex>::Iterator v, Graph &graph,
                     List<List<Vertex>> const &polygons, int &loopCounter,
                     int &visibleCounter) {
    
    // First vertex after v
    List<Vertex>::Iterator firstCheck = v;
    ++firstCheck;
    
    // Traversal by Iterator
    List<Vertex>::Iterator end = graph.vertices.end();
    
    // Loop through higher-indexed vertices above v
    for (List<Vertex>::Iterator check = firstCheck; check != end; ++check) {
        
        if (visible(**v, **check, polygons, visibleCounter)) {
            
            // check is visible from v and vice versa, build an edge
            double distance = distanceFormula(**v, **check, DIMENSIONS);
            
            // Give each new edge pointers to vertices
            // NOTE: vertices in edge are owned by graph's vertices list
            Edge* newEdge = new Edge{*v, *check, distance};
            
            //Place edge in graph's list
            graph.connections.insertEnd(newEdge);
        }
        ++loopCounter;
    }
}

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
                     int &visibleCounter, List<Edge> &victims) {
    
    // First vertex after v
    List<Vertex>::Iterator firstCheck = v;
    ++firstCheck;
    
    // Traversal by Iterator
    List<Vertex>::Iterator end = graph.vertices.end();
    
    // Loop through higher-indexed vertices above v
    for (List<Vertex>::Iterator check = firstCheck; check != end; ++check) {
        
        if (visible(**v, **check, polygons, visibleCounter)) {
            
            // check is visible from v and vice versa, build an edge
            double distance = distanceFormula(**v, **check, DIMENSIONS);
            
            // Give each new edge pointers to vertices
            // NOTE: vertices in edge are owned by graph's vertices list
            Edge* newEdge = new Edge{*v, *check, distance};
            
            //Place edge in graph's list
            graph.connections.insertEnd(newEdge);
            
            //Also place edge in victims list
            victims.insertEnd(newEdge);
        }
        ++loopCounter;
    }
}

// REQUIRES: v and check are valid vertices; v != check;
//           v and check are not in the interior of a polgon
//           polygons contains valid polygon objects
// EFFECTS : returns true if check is visible from v (the line segment
//           connecting check and v does intersect any polygon edges);
//           returns false otherwise. Adds the number of loops run to
//           loopCounter
bool visible(const Vertex& v, const Vertex& check,
             List<List<Vertex>> const &polygons, int &loopCounter) {

    // Bug Fix (Oct 29, 2017): Each vertex now carries a polygon index. If these
    // are equal, the vertices are part of the same polygon. Then, check if they
    // are adjacent. If so, they are visibile from each other. If not, they are
    // not visible from each other. This limits the program to CONVEX polygons.
//    if (v.polygon == check.polygon) {
//        Polygon* samePgon = polygons.at(v.polygon);
//
//        // Find the corresponding vertices to v and check in samePgon
//        bool vFound = false;
//        bool checkFound = false;
//        const Vertex* vPtr = nullptr;
//        const Vertex* checkPtr = nullptr;
//
//        // First vertex in samePgon
//        const Vertex* v1 = samePgon->polygonVertices.firstItem();
//        if (*v1 == v) {
//            vFound = true;
//            vPtr = v1;
//        }
//        if (*v1 == check) {
//            checkFound = true;
//            checkPtr = v1;
//        }
//
//        for (int i = 0; (i < samePgon->polygonVertices.size()) && !vFound
//                         && !checkFound; ++i) {
//
//            if (*v1 == v) {
//                vFound = true;
//                vPtr = v1;
//            }
//            if (*v1 == check) {
//                checkFound = true;
//                checkPtr = v1;
//            }
//
//            // Increment to next vertex in samePgon
//            v1 = samePgon->polygonVertices.nextItem(v1);
//        }
//
//        // v and check should have been found
//        assert(vFound && checkFound);
//
//        // Adjacent check
//        return (samePgon->polygonVertices.nextItem(vPtr) == checkPtr ||
//                samePgon->polygonVertices.nextItem(checkPtr) == vPtr);
//    }
    // End Bug Fix Oct 29, 2017
    
    // Simpler Bug Fix Oct 30, 2017
    
    // Check if from same polygon
    if (v.polygon == check.polygon && v.polygon != -1) {
        
        ++loopCounter;
        
        // Check if adjacent
        return (abs(v.location - check.location) == 1 ||
            (v.location == 0 && check.location == check.poly_size - 1) ||
            (check.location == 0 && v.location == v.poly_size - 1));
    }
    
    // End Bug Fix Oct 30, 2017

    // Traversal by Iterator
    List<List<Vertex>>::Iterator endP = polygons.end();
    // Check all polygons
    for (List<List<Vertex>>::Iterator pgon = polygons.begin(); pgon != endP;
         ++pgon) {
        
        // Iterator to first vertex in pgon
        List<Vertex>::Iterator v1 = (*pgon)->begin();
        
        // Traversal by index
        for (int j = 0; j < (*pgon)->size(); ++j) {
            
            // Get vertex that comes after v1. If v1 is last item in list, v2
            // will be first vertex. Thus [v1, v2] represent all edges
            List<Vertex>::Iterator v2 = v1;
            v2.circularIncrement((*pgon)->begin());
            
            ++loopCounter;
            
            if (intersect(v, check, **v1, **v2)) {
                return false;
            }
            
            // Increment to next vertex
            v1 = v2;
        }
    }
    
    // No intersections found
    return true;
}

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
               Vertex const& b1, Vertex const& b2) {
    // Bug fix: Return false in special case where line segments have a common
    //          endpoint
    if (a1 == b1 || a1 == b2 || a2 == b1 || a2 == b2) {
        return false;
    }
    
    // Get 4 orientations of interest
    int o1 = orientation(a1, a2, b1);
    int o2 = orientation(a1, a2, b2);
    int o3 = orientation(b1, b2, a1);
    int o4 = orientation(b1, b2, a2);
    
    // Examples 1 and 2 of general case
    if (o1 != o2 && o3 != o4) {
        return true;
    }
    // All others
    return false;
}

// EFFECTS : Returns true if lhs and rhs have the same coordinates.
bool operator==(const Vertex &lhs, const Vertex &rhs) {
    for (int i = 0; i < DIMENSIONS; ++i) {
        if (!compareDoubles(lhs.coord[i], rhs.coord[i])) {
            return false;
        }
    }
    // All coordinates were equal
    return true;
}

// REQUIRES: v is a valid Vertex
// MODIFIES: os
// EFFECTS : Prints the Vertex to os (coordinates only)
std::ostream & operator<<(std::ostream &os, const Vertex &v) {
    //os << "(";
    for (int i = 0; i < DIMENSIONS - 1; ++i) {
        //os << v.coord[i] << ", ";
        os << v.coord[i] << " ";
    }
    //os << v.coord[DIMENSIONS - 1] << ")";
    os << v.coord[DIMENSIONS - 1];
    return os;
}

// REQUIRES: e is a valid Edge
// MODIFIES: os
// EFFECTS : Prints the Edge to os (Vertices only)
std::ostream & operator<<(std::ostream &os, const Edge &e) {
    os << "V1: " << *(e.v1) << " V2: " << *(e.v2);
    
    return os;
}

// REQUIRES: g is a valid Graph
// MODIFIES: os
// EFFECTS : Prints the vertices and connections lists to os
std::ostream & operator<<(std::ostream &os, const Graph &g) {
    os << "Vertices:" << endl << g.vertices << endl;
    os << "Connections:" << endl << g.connections << endl;
    
    return os;
}
