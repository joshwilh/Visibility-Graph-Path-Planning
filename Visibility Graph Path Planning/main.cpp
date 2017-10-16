//
//  main.cpp
//  Visibility Graph Path Planning
//
//  Created by Joshua Wilhelm on 10/14/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include "List.h"

using namespace std;

// REQUIRES: polygons is an empty vector of vectors of doubles, 'is' is a valid
//           input stream that has been opened and is in the correct format
// MODIFIES: polygons
// EFFECTS : reads vertices from is and places them into polygons
void read_polygons(vector<vector<double> >& polygons, istream& is);

//TODO: Write main
int main(int argc, const char * argv[]) {
    
    // Take in polygon file
    cout << "Enter a filename (polygon obstacles): ";
    string polygon_filename;
    cin >> polygon_filename;
    cout << endl;
    
    // Check if polygon file opens properly and send to reading function
    ifstream polygon_fin;
    polygon_fin.open(polygon_filename);
    
    if(!polygon_fin){
        cout << "Error opening file: " << polygon_filename << endl;
        return 1;
    }

    vector<vector<double> > polygons;
    read_polygons(polygons, polygon_fin);
    
    polygon_fin.close();
    
    // Testing List
    /*
     * List<int> testList;
     * int x = 5;
     * testList.insertStart(&x);
     * cout << testList.removeEnd() << endl;
     * testList.insertByValue(&x); // Should cause compiler error
     */
     
    return 0;
}

// REQUIRES: polygons is an empty vector of vectors of doubles, 'is' is a valid
//           input stream that has been opened and is in the correct format
// MODIFIES: polygons
// EFFECTS : reads vertices from is and places them into polygons
void read_polygons(vector<vector<double> >& polygons, istream& is) {
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
