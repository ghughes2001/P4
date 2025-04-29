/*
Author: Grant Hughes
Date: April 22, 2025

ASMCode.cpp:
    - Implementation for generating assembly code
*/

#include <iostream>
#include <cstdlib>
#include <string>

#include "ASMCode.hpp"

using namespace std;

// helper functions converted to C++ from C
// inline function for termination:
inline void fatal(const string& message) {
    cerr << "\nFatal: " << message << "\n\n";
    exit(1);
}

// function definitions:
string t2_2_pID(string t2) {
    if (t2[0] != '+') 
        fatal("Expected a plus sign for the first character of a t2 token.");
    t2[0] = 'p';
    
    return t2;
}

int t3_2_intVal(const string& t3) {
    int intVal; // will hold the integer value of the t3 token
    int caseVal; // set to +1 if upper-case (positive value) and set to -1 if lower-case (negative value)
    
    if ((t3[0] < 65) || (t3[0] > 122)) // checking whole range from A to Z (ascii 65 - 122)
        fatal("Expected a letter for the first character of a t3 token.");
    if ((t3[0] > 90) && (t3[0] < 97)) // checking for ascii non-alphabetical characters between Z and a (ascii 91 - 96)
        fatal("Expected a letter for the first character of a t3 token.");
    if ((t3[0] > 64) && (t3[0] < 91)) // upper-case letter, so integer will be positive
        caseVal = 1;
    if ((t3[0] > 96) && (t3[0] < 123)) // lower-case letter, so integer will be negative
        caseVal = -1;
    
    // Create a copy of t3 with first character replaced by '0'
    string t3Copy = t3;
    t3Copy[0] = '0';
    
    intVal = stoi(t3Copy); // convert to integer
    intVal = intVal * caseVal; // set to negative if needed   
    
    return intVal;
}

int ASMCode::processTokenThree(const string& token)
{
    return t3_2_intVal(token);
}

string ASMCode::processTokenTwo(const string& token)
{
    return t2_2_pID(token);
}

// method to add temp variables
string ASMCode::createTempVariables()
{
    string temp = "temp" + to_string(countForTempVariables);
    usedVariables.push_back(temp); // adding temp variables to vector of total varibles

    return temp;
}

