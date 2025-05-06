/*
Author: Grant Hughes
Date: April 22, 2025

ASMCode.cpp:
    - Implementation for generating assembly code
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>

#include "ASMCode.hpp"

using namespace std;

// prototypes
void processEachNodeInTree(node* root);

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
    int caseVal = 0; // set to +1 if upper-case (positive value) and set to -1 if lower-case (negative value)
    
    if ((t3[0] < 65) || (t3[0] > 122)) // checking whole range from A to Z (ascii 65 - 122)
        fatal("Expected a letter for the first character of a t3 token.");
    if ((t3[0] > 90) && (t3[0] < 97)) // checking for ascii non-alphabetical characters between Z and a (ascii 91 - 96)
        fatal("Expected a letter for the first character of a t3 token.");
    if ((t3[0] > 64) && (t3[0] < 91)) // upper-case letter, so integer will be positive
        caseVal = 1;
    else if ((t3[0] > 96) && (t3[0] < 123)) // lower-case letter, so integer will be negative
        caseVal = -1;
    else
        fatal("Unexpected character value in t3 token");
    
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
string ASMCode::createExtraTempVariables()
{
    string temp = "temp" + to_string(countForTempVariables);
    usedVariables.push_back(temp); // adding extra temp variables to vector of total varibles

    return temp;
}

void ASMCode::generateA(node* node)
{
    // checking if there is an A node
    if (node->child.empty() || node->child[0]->tokenType == "EMPTY")
    {
        return;
    }
    if (node->child[0]->tokenInstance == "\"")
    {
        // allocate memory for t2 and initialize its value to zero
        if (node->child.size() > 1 && node->child[1]->tokenType == "t2")
        {
            string variable = processTokenTwo(node->child[1]->tokenInstance);
            // add token to variable list
            usedVariables.push_back(variable);
            output << "LOAD 0" << endl;
            output << "STORE " << variable << endl;
        }
    }
}

void ASMCode::generateB(node* node)
{
    // checking if node there is a B node
    if (node->child.empty() || node->child[0]->tokenType == "EMPTY")
    {
        return;
    }
    // otherwise return back to processing method for S | C | D | E | G
    processEachNodeInTree(node->child[0]);
}

void ASMCode::generateC(node* node)
{
    // checking if there is a C node
    if (node->child.empty())
    {
        return;
    }
    if (node->child[0]->tokenInstance == "#")
    {
        // allocate memory for the identifier, read in a number, and assign the value to the identifier
        if (node->child.size() > 1 && node->child[1]->tokenType == "t2")
        {
            string variable = processTokenTwo(node->child[1]->tokenInstance);
            // add to list of variables
            usedVariables.push_back(variable);
            // now write READ in assembly
            output << "READ" << variable << endl;
        }
    }
    else if (node->child[0]->tokenInstance == "!") // C grammer can have the negate rule
    {
        // need to multiply the value by -1 to negate it
        if (node->child.size() > 1)
        {
            string negateVariable = createExtraTempVariables();
            // now looking at F grammar
            generateF(node->child[1], negateVariable);
            // store in assmebly file
            output << "STORE " << negateVariable << endl;
            // Load the value, negate it, and store it back
            output << "LOAD " << negateVariable << endl;
            output << "MULT -1" << endl;
            output << "STORE " << negateVariable << endl;
        }
    }
}

void ASMCode::generateD(node* node)
{
    // checking if there is D node or $
    if (node->child.empty() || node->child[0]->tokenInstance != "$") {
        return;
    }
    // print out the value of the identifier or immediate number to the screen
    if (node->child.size() > 1)
    {
        string variable = createExtraTempVariables();
        generateF(node->child[1], variable);
        // write it in assembly file
        output << "WRITE" << variable << endl;
    }
}

void ASMCode::generateE(node* node)
{
    // checking if there is no E node or '
    if (node->child.empty() || node->child[0]->tokenInstance != "'") {
        return;
    }
    // the first argument is strictly greater than the second argument, repeat the
    // fourth argument n times, where n is the third argument
    if (node->child.size() >= 5)
    {
        // first argument
        string firstArgument = createExtraTempVariables();
        generateF(node->child[1], firstArgument);
        // second argument
        string secondArgument = createExtraTempVariables();
        generateF(node->child[2], secondArgument);

        // Creating loop names for assembly code
        string startingLoop = "start" + to_string(countForTempVariables);
        string endingLoop = "end" + to_string(countForTempVariables);
        countForTempVariables++;

        // third argument (determines how many times the loop repeats)
        string thirdArgument = createExtraTempVariables();
        generateF(node->child[3], thirdArgument);

        // Check if loop count > 0 otherwisie why loop
        output << "LOAD " << thirdArgument << endl;
        output << "BRZNEG " << endingLoop << endl;
        
        // comparing first argument
        output << "LOAD " << firstArgument << endl;
        output << "SUB " << secondArgument << endl;
        output << "BRZNEG " << endingLoop << endl;
        
        // begin counter since greater
        output << "LOAD " << thirdArgument << endl;
        output << "STORE " << thirdArgument << endl;
        
        // Loop start label
        output << startingLoop << ": LOOP" << endl;

        // B node
        processEachNodeInTree(node->child[4]);

        // descrease the third argument
        output << "LOAD " << thirdArgument << endl;
        output << "SUB 1" << endl;
        output << "STORE " << thirdArgument << endl;
        
        // Check if loop counter > 0
        output << "BRPOS " << startingLoop << endl;
        
        // Loop end label
        output << endingLoop << ": LOOP" << endl;
    }
}

void ASMCode::generateF(node* node, string& variable)
{
    // checking if there is no F node
    if (node->child.empty())
    {
        return;
    }
    // varaibles to access the what the node is and it's token type
    string nodeTokenType = node->child[0]->tokenType;
    string nodeTokenInstance = node->child[0]->tokenInstance;

    if (nodeTokenType == "t2")
    {
        // add as varibale and write to assembly file
        string insideVariable = processTokenTwo(nodeTokenInstance);
        output << "LOAD " << insideVariable << endl;
    }
    if (nodeTokenType == "t3")
    {
        // add as varibale and write to assembly file
        int value = processTokenThree(nodeTokenInstance);
        output << "LOAD " << value << endl;
    }
    else if (nodeTokenType == "t1" && nodeTokenInstance == "&")
    {
        // & +24 R100 means to add the value for identifier +24 plus 100
        if (node->child.size() >= 3)
        {
            // getting first arguemnt
            string firstArguemnt = createExtraTempVariables();
            generateF(node->child[1], firstArguemnt);
            output << "STORE " << firstArguemnt << endl;
            
            // gettoing second argument
            string secondArgument = createExtraTempVariables();
            generateF(node->child[2], secondArgument);
            
            // Add the first arguemnt to second arguemnt
            output << "ADD " << firstArguemnt << endl;
        }
    }
}

void ASMCode::generateG(node* node)
{
   // handling the assignment operator (%)
    if (node->child.size() < 3 || node->child[1]->tokenInstance != "%")
    {
        return;
    }
    
    // FIX: Proper assignment implementation - load right side, store to left side
    
    // Getting the left-side (target) variable
    string leftVariable = processTokenTwo(node->child[0]->tokenInstance);
    
    // Add left variable to global variables if not already there
    if (find(usedVariables.begin(), usedVariables.end(), leftVariable) == usedVariables.end()) {
        usedVariables.push_back(leftVariable);
    }
    
    // Evaluate the right-side expression (F node)
    generateF(node->child[2], leftVariable);
    
    // Store the result in the left variable
    output << "STORE " << leftVariable << endl;
}

// method to look at nodes and tree and know which grammar to call
void ASMCode::processEachNodeInTree(node* node)
{
    if (node == nullptr)
    {
        return; // there is no node to look at
    }
    string tokenTypeOfNode = node->tokenType; // access the node that is is tree

    // looking at node based on non-termianl in grammer
    if (tokenTypeOfNode == "S")
    {
        for (auto child: node->child)
        {
            processEachNodeInTree(child);
        }
    }
    else if (tokenTypeOfNode == "A")
    {
        generateA(node);
    }
    else if (tokenTypeOfNode == "B")
    {
        generateB(node);
    }
    else if (tokenTypeOfNode == "C")
    {
        generateC(node);
    }
    else if (tokenTypeOfNode == "D")
    {
        generateD(node);
    }
    else if (tokenTypeOfNode == "E")
    {
        generateE(node);
    }
    else if (tokenTypeOfNode == "G")
    {
        generateG(node);
    }
}

ASMCode::ASMCode(const std::string file, SymbolTable& symbolTable): countForTempVariables(0), table(symbolTable), output(file) {
    if (!output.is_open()) {
        cout << "Error: Could not open output file for assembly code creation." << endl;
        exit(1);
    }
}

void ASMCode::generateASM(node* node)
{
    // check if node is empty
    if (node == nullptr)
    {
        return;
    }
    // begin proccessing the tree for assembly code
    processEachNodeInTree(node);
    // write STOP in assembly file once done
    output << "STOP" << endl;
    // now wrting variables that were used
    writeToASM();

}

// mehtod to write all temp variables
void ASMCode::writeToASM()
{
    for (string tempVariable: usedVariables)
    {
        output << tempVariable << " 0" << endl; // write temp variable and assign it's value 0
    }
}