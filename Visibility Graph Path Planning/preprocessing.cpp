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

// REQUIRES: all parameters are valid vertices
// EFFECTS : returns 1 if orientation of (p1, p2, p3) is clockwise
//           returns -1 if orientation of (p1, p2, p3) is counterclockwise
//           returns 0 if (p1, p2, p3) are colinear
// NOTE    : Helper function for intersect
//           Credit: http://www.geeksforgeeks.org/orientation-3-ordered-points/
static int orientation (Vertex const p1, Vertex const p2, Vertex const p3) {
    // Orientation formula: (y2 - y1)*(x3 - x2) - (y3 - y2)*(x2 - x1)
    int result = (p2.yPos - p1.yPos) * (p3.xPos - p2.xPos) - (p3.yPos - p2.yPos)
    * (p2.xPos - p1.xPos);
    if (result == 0) {
        return 0;
    }
    if (result > 0) {
        return 1;
    }
    return -1;
}

// REQUIRES: polygons is an empty vector of vectors of doubles, 'is' is a valid
//           input stream that has been opened and is in the correct format
// MODIFIES: polygons
// EFFECTS : reads vertices from is and places them into polygons
static void read_polygons(vector<vector<double> >& polygons, istream& is) {
    assert(polygons.size() == 0);
    
    int num_vertices;
    
    // Reads one polygon per loop
    while (is >> num_vertices) {
        
        // Reads all vertex coordinates for this polygon
        vector<double> vertices;
        for (int i = 0; i < 2 * num_vertices; ++i) { // 2 coord per vertex
            double read_vertex;
            is >> read_vertex;
            vertices.push_back(read_vertex);
        }
        
        polygons.push_back(vertices);
    }
}

// REQUIRES: graph is an empty graph, polygonFile has been opened properly and
//           contains polygons in the proper format, polygons cannot overlap,
//           nor can they share vertices, edges, or have a vertex on the edge of
//           another polygon
// MODIFIES: graph, polygonFile
// EFFECTS : reads polygonFile, then calls addVerticesAndEdges, then
//           makeConnections (which calls visibleVertices on each vertex)
void preProcess(Graph &graph, std::istream& polygonFile) {
    
    vector<vector<double> > polygons;
    read_polygons(polygons, polygonFile);
    
    vector<Edge> polygonEdges;
    addVerticesAndEdges(graph, polygons, polygonEdges);
    
    makeConnections(graph, polygonEdges);
}

// REQUIRES: graph is an empty Graph, polygons contains vectors of polygon
//           coordinates in the correct format, polygonEdges is an empty vector.
// MODIFIES: graph, polygonEdges
// EFFECTS : all of the vertices in polygons are added to graph with empty
//           'connections' lists, fills polygonEdges with edges to be used in
//           the visible function
void addVerticesAndEdges(Graph &graph,
                         std::vector<std::vector<double> > const &polygons,
                         std::vector<Edge> &polygonEdges) {
    // Check graph and polygonEdges are empty
    assert(graph.vertices.empty());
    assert(polygonEdges.empty());
    
    // Make empty list to add to graph
    List<Vertex> vertices;
    graph.vertices = vertices;
    
    for (int polygon = 0; polygon < polygons.size(); ++polygon) {
        
        // Can't have a polygon with fewer than 3 vertices (6 coordinates)
        assert(polygons[polygon].size() >= 6);
        
        // Add first vertex, keep in storage between loops
        Vertex* newVert = new Vertex;
        newVert->xPos = polygons[polygon][0];
        newVert->yPos = polygons[polygon][1];
        graph.vertices.insertEnd(newVert);
        
        Vertex currentVertex = *newVert;
        
        for (int j = 2; j < polygons[polygon].size() - 1; j += 2) {
            // Each vertex in this polygon after the first
            
            Vertex *newVert1 = new Vertex;
            newVert1->xPos = polygons[polygon][j];
            newVert1->yPos = polygons[polygon][j+1];
            graph.vertices.insertEnd(newVert1);
            
            // Each edge except the last (from last vertex to first vertex)
            Edge newEdge = {currentVertex, *newVert1};
            polygonEdges.push_back(newEdge);
            
            currentVertex = *newVert1;
        }
        
        // Create edge that goes from last vertex (currentVertex) in list to
        // first vertex (*newVert)
        Edge newEdge = {currentVertex, *newVert};
        polygonEdges.push_back(newEdge);
    }
}

// REQUIRES: graph and polygonEdges have been successfully passed through
//           addVerticesAndEdges.
//           contains vectors of polygon coordinates in the correct format.
// MODIFIES: graph
// EFFECTS : Checks each vertex in graph for all visible vertices, and adds all
//           visible pairs as edges in graph
void makeConnections(Graph &graph, std::vector<Edge> const &polygonEdges) {
    
    Vertex* v = graph.vertices.firstItem();
    
    while (v != nullptr) {
        visibleVertices(v, graph, polygonEdges);
        v = graph.vertices.nextItem(v);
    }
}

// REQUIRES: v is a vertex in graph. graph and polygonEdges have been
//           successfully passed through addVerticesAndEdges, v is not in the
//           interior of a polygon
// MODIFIES: *v, graph
// EFFECTS : adds all visible vertices from v that are indexed higher (listed
//           later) in graph to v's connections as edges, and adds v to those
//           vertices' connections lists, in both cases with distances
void visibleVertices(Vertex *v, Graph &graph,
                     std::vector<Edge> const &polygonEdges) {
    Vertex * check = graph.vertices.nextItem(v);
    
    // Loop through higher-indexed vertices above v
    while (check != nullptr) {
        if (visible(*v, *check, polygonEdges)) {
            // check is visible from v and vice versa, build an edge
            double distance = sqrt(pow(check->xPos - v->xPos, 2) +
                                   pow(check->yPos - v->yPos, 2));
            Edge* newEdge = new Edge;
            *newEdge = {*v, *check, distance};
            
            //Place edge in both vertices' connections lists
            v->connections.insertEnd(newEdge);
            check->connections.insertEnd(newEdge);
        }
        
        // Increment check to next vertex
        check = graph.vertices.nextItem(check);
    }
}

// REQUIRES: v and check are valid vertices; v != check;
//           v and check are not in the interior of a polgon
//           polygonEdges has been properly constructed in generateEdges
// EFFECTS : returns true if check is visible from v (the line segment
//           connecting check and v does intersect any polygon edges);
//           returns false otherwise
bool visible(Vertex const v, Vertex const check,
             std::vector<Edge> const polygonEdges) {
    
    // Check all polygon edges for intersection
    for (int i = 0; i < polygonEdges.size(); ++i) {
        if (intersect(v, check, polygonEdges.at(i).v1, polygonEdges.at(i).v2)) {
            return false;
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
//                 Special Case Example 1 returns false because flying along
//                 edges is allowed. Example 2 remains the same.
bool intersect(Vertex const a1, Vertex const a2,
               Vertex const b1, Vertex const b2) {
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
