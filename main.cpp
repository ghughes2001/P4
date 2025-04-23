/*
Author: Grant Hughes
Created: February 23, 2025

Main.cpp:
    - Process arguments
    - Calls testSCanner()
*/

#include <iostream>
#include <cstdlib> // exit()
#include <fstream> // file handling
#include <string>

#include "parser.hpp"
#include "testTree.hpp"
#include "semantics.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    // adding filename to get either input from user or from command line
    string fileName;

    // checking if there is more than 1 filename present
    // And printing erro message if so (must be only 1 file)
    if (argc >= 3)
    {
        cout << "Fatal: Imroper Usuage" << endl;
        cout << "Usuage: P0 [filename]" << endl;
        
        // Terminate program if this is encountered
        exit(1);
    }
    // if 1 file then ooen and read contents
    // And build tree (if possible)
    if (argc == 2)
    {
        // variables
        fileName = argv[1];

        ifstream myFile(fileName);
        if (!myFile.is_open())
        {
            cout << "Error: File could not be found" << endl;
            exit(1);
        }
        myFile.close();
    }
    if (argc == 1)
    {
        // variables for user input
        string inputLine;
        ofstream writeFile("output.txt", ios::trunc);

        if (!writeFile.is_open())
        {
            cout << "Error: Could not open file for writing" << endl;
            exit(1);
        }

        while (getline(cin, inputLine))
        {
            if (inputLine.empty())
                continue;
            writeFile << inputLine << endl;
        }
        writeFile.close();

        fileName = "output.txt";

    }
    // calling parser
    node* tokenTree = (node*) parser(fileName);
    // checking semantics
    SymbolTable s;
    s.checkTable(tokenTree);
    // display the table on screen
    s.printTable();
    // cleaning memory
    deleteTree(tokenTree);

    if (argc == 1) // cleaning user input
    {
        // erasing contents of file so the next user dosn't have it
        ofstream fileToClear("output.txt", ios::out | ios::trunc);
        fileToClear.close();
    }

    return 0;
}