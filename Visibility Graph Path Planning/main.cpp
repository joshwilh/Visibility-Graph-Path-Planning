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
#include "preprocessing.h"

using namespace std;

//TODO: Write main
int main(int argc, const char * argv[]) {
    
    // Take in polygon file
    cout << "Enter a filename (polygon obstacles): ";
    string polygon_filename;
    cin >> polygon_filename;
    cout << endl;
    
    // Check if polygon file opens properly
    ifstream polygon_fin;
    polygon_fin.open(polygon_filename);
    
    if(!polygon_fin){
        cout << "Error opening file: " << polygon_filename << endl;
        return 1;
    }
    
    // Testing Preprocessing (see README)
    Graph g;
    preProcess(g, polygon_fin);
    
    polygon_fin.close();
    
    // Testing List
//    List<int> testList;
//    int* x = new int(6);
//    testList.insertStart(new int(5));
//    testList.insertEnd(x);
//    testList.insertEnd(new int(7));
//    testList.insertEnd(new int(8));
//    int* y = testList.nextItem(x);
//    assert(*y == 7);
//    y = testList.nextItem(y);
//    y = testList.nextItem(y);
//    assert(y == nullptr);
//    delete testList.removeStart();
//    delete testList.removeStart();
//    delete testList.removeStart();
//    delete testList.removeStart();
//    cout << "List tests passed!" << endl;
    //cout << testList.removeEnd() << endl;
    //testList.insertByValue(&x); // Should cause compiler error
     
    return 0;
}
