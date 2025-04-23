/*
Author: Grant Hughes
Created: March 26, 2025

testTree.cpp:
    - Implementation for printing the parse tree
*/

#include <iostream>
#include <string>

#include "testTree.hpp"

using namespace std;

void printTree(node* root, int indent)
{
    // base case
    if (root == nullptr)
        return;
    
    // Printing indentation
    for (int i = 0; i < indent; i++)
    {       
        cout << " ";
    }
    
    // Print node type
    cout << root->tokenType;

    // Print token instance if it exists
    if (!root->tokenInstance.empty())
    {
        cout << " " << root->tokenInstance;
    }
    cout << endl;

    // Recursively print children with increased indentation
    for (node* child : root->child)
    {
        printTree(child, indent + 4);
    }
}

void deleteTree(node* root)
{
    if (root == nullptr)
        return;
    
    // Recursively delete all children first
    for (node* child : root->child)
    {
        deleteTree(child);
    }
    
    // Then delete the current node
    delete root;
}