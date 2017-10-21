//
//  preprocessing.cpp
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 10/16/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <stdio.h>
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

// REQUIRES: graph is an empty Graph, polygons contains vectors of polygon
//           coordinates in the correct format.
// MODIFIES: graph
// EFFECTS : all of the vertices in polygons are added to graph with empty
//           'connections' lists
void addVertices(Graph &graph,
                 vector<std::vector<double> > const &polygons) {
    // Make empty list to add to graph
    List<Vertex> vertices;
    graph.vertices = vertices;
    
    for (int i = 0; i < polygons.size(); ++i) {
        for (int j = 0; j < polygons[i].size(); j += 2) {
            Vertex *newVert = new Vertex;
            newVert->xPos = polygons[i][j];
            newVert->yPos = polygons[i][j+1];
            graph.vertices.insertEnd(newVert);
        }
    }
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

// REQUIRES: v and check are valid vertices; v != check;
//           v and check are not in the interior of a polgon
// EFFECTS : returns true if check is visible from v (the line segment
//           connecting check and v does intersect any polygon edges);
//           returns false otherwise
bool visible(Vertex const v, Vertex const check,
             std::vector<std::vector<double> > const &polygons) {
    vector<vector<Vertex> > edges;
    for (int polygon = 0; polygon < polygons.size(); ++polygon) {
        // Build edges
        for (int i = 0; i < polygons[polygon].size() - 3; i += 2) {
            Vertex e1 = {polygons[polygon][i], polygons[polygon][i+1]};
            Vertex e2 = {polygons[polygon][i+2], polygons[polygon][i+3]};
            vector<Vertex> edge = {e1, e2};
            edges.push_back(edge);
        }
        // Create edge that goes from last vertex in list to first vertex
        Vertex e1 = {polygons[polygon][0], polygons[polygon][1]};
        Vertex e2 = {polygons[polygon][polygons.size() - 2],
                     polygons[polygon][polygons.size() - 1]};
        vector<Vertex> edge = {e1, e2};
        edges.push_back(edge);
    }
    
    // Check all edges for intersection
    for (int i = 0; i < edges.size(); ++i) {
        if (intersect(v, check, edges.at(i).at(0), edges.at(i).at(1))) {
            return false;
        }
    }
    
    // No intersections found
    return true;
}
