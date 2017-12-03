//
//  postprocess.cpp
//  postprocess
//
//  Created by Joshua Wilhelm on 12/2/17.
//  Copyright © 2017 Joshua Wilhelm. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

// REQUIRES: 'is' is a vaild input stream at the beginning of a testing output
//            file. 'os' is a valid output stream.
// MODIFIES: is, os
// EFFECTS : Processes the visibility graph (pre-process) section of the output
//           to a MATLAB-friendly format (see README).
void visGraphHelper(istream &is, ostream &os);

// REQUIRES: 'is' is a vaild input stream at the beginning of a testing output
//            file. 'os' is a valid output stream.
// MODIFIES: is, os
// EFFECTS : Prints the number of successes and the number of failures in A* to
//           os
void AStarSuccessFailure(istream &is, ostream &os);

// REQUIRES: 'is' is a vaild input stream at the beginning of a testing output
//            file. 'os' is a valid output stream.
// MODIFIES: is, os
// EFFECTS : Processes the A* section of the output to a MATLAB-friendly format
//           (see README).
void AStarHelper(istream &is, ostream &os);

int main() {
    
    // Take in outupt file
    cout << "Enter a filename (to process): ";
    string toProcessFilename;
    cin >> toProcessFilename;
    cout << endl;

    // Check if output file opens properly
    ifstream toProcessFin(toProcessFilename);

    if(!toProcessFin){
        cout << "Error opening file: " << toProcessFilename << endl;
        return 1;
    }
    
    // Create a postprocessed file
    ofstream postProccessFout("post_process_vis_graph.out");
    
    // Send to helper function
    visGraphHelper(toProcessFin, postProccessFout);
    toProcessFin.close();
    postProccessFout.close();
    
    toProcessFin.open(toProcessFilename);
    postProccessFout.open("post_process_AStar.out");
    AStarSuccessFailure(toProcessFin, postProccessFout);
    toProcessFin.close();
    toProcessFin.open(toProcessFilename);
    AStarHelper(toProcessFin, postProccessFout);
    
    // Close file streams
    toProcessFin.close();
    postProccessFout.close();
    
    return 0;
}

void visGraphHelper(istream &is, ostream &os) {
    string input;
    // Discard the first "test_0000"
    is >> input;
    
    while (is.good()) {
        for (int i = 0; i < 3; ++i) {
            // Discard 3 values for polygon generation
            is >> input;
        }
        
        for (int i = 0; i < 8; ++i) {
            // These 8 values are those of interest (see README)
            is >> input;
            os << input << " ";
        }
        
        // This row is complete
        os << endl;
        
        // Read to next "test_####" statement, or end of file
        while (is >> input) {
            if (input.substr(0, 4) == "test") {
                break;
            }
        }
    }
}

void AStarSuccessFailure(istream &is, ostream &os) {
    string input;
    int successCount = 0;
    int failCount = 0;
    
    while (is >> input) {
        if (input == "Success!") {
            ++successCount;
        } else if (input == "Failure") {
            ++failCount;
        }
    }
    
    os << successCount << " " << failCount << " " << endl;
}

void AStarHelper(istream &is, ostream &os) {
    string input;
    
    // Read through file
    while (is >> input) {
        // Read until Success found
        if (input == "Success!") {
            // Note: the commented-out code does not work because doubles are
            //       not necessarily printed with a decimal
//            // Get lines. 2 decimals indicate part of the solution path. 1
//            // indicates solution distance. None indicate we now have values of
//            // interest. 4 or "test_####" indicate we have gone too far (direct
//            // visibility occurred).
//            size_t numOfDec = -1;
//            while (getline(is, input)) {
//                // If the line is empty read the next one
//                if (input.length() == 0) {
//                    continue;
//                }
//                numOfDec = count(input.begin(), input.end(), '.');
//                if (numOfDec == 0 || numOfDec == 4 ||
//                    input.substr(0, 4) == "test") {
//                    break;
//                }
//            }
//
//            // If the above loop was exited because 4 decimals were found, skip
//            // to next search because this search was a direct visibility case
//            if (numOfDec == 4 || input.substr(0, 4) == "test") {
//                continue;
//            }
            
            // New attempt, counting spaces
            size_t numOfSpace = -1;
            while (getline(is, input)) {
                // If the line is empty read the next one
                if (input.length() == 0) {
                    continue;
                }
                // If this line contains no spaces, it is the solution distance
                numOfSpace = count(input.begin(), input.end(), ' ');
                if (numOfSpace == 0) {
                    break;
                }
            }
            // Read the line after the solution distance
            getline(is, input);
            // If this line contains exactly one space, it is the beginning of
            // an A* result. Otherwise, it is not
            if (count(input.begin(), input.end(), ' ') != 1) {
                continue;
            }
            
            // The two values in input are of interest (see README)
            os << input << " ";
            
            for (int i = 0; i < 2; ++i) {
                // These 2 values are also of interest (see README)
                is >> input;
                os << input << " ";
            }
            
            // This row is complete
            os << endl;
        }
    }
}
