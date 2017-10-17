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

// REQUIRES: graph is an empty Graph, polygons contains vectors of polygon
//           coordinates in the correct format.
// MODIFIES: graph
// EFFECTS : all of the vertices in polygons are added to graph with empty
//           'connections' lists
void addVertices(Graph &graph,
                 std::vector<std::vector<double> > const &polygons);

#endif /* preprocessing_h */
