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
