/*
Auhtor: Grant Hughes
Date: April 13, 2025

semantics.hpp
    - Implementation of semantics class (needs to be more private than the other parts of project)
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "semantics.hpp"
#include "node.hpp"

using namespace std;

bool SymbolTable::verify(string var)
{
    return find(symbol.begin(), symbol.end(), var) != symbol.end();
}

bool SymbolTable::insert(string var)
{
    if (!verify(var)) // if the string is not in table, then insert it
    {
        symbol.push_back(var);
        return true;
    }
    return false;
}

// function to look and see if non-terminals w/ " or # were created as a node
bool nonTermCreated(node* nonTermNode)
{
    // looking at the A non-terminal node
    if (nonTermNode->tokenType == "A" && !nonTermNode->child.empty() && nonTermNode->child[0]->tokenType == "t1")
    {
        if (nonTermNode->child[0]->tokenInstance == "\"" || nonTermNode->child[0]->tokenInstance == "#")
        {
            return true;
        }
    }

    // looking at the C non-temrinal node
    if (nonTermNode->tokenType == "C" && !nonTermNode->child.empty())
    {
        if (nonTermNode->child[0]->tokenType =="t1" && nonTermNode->child[0]->tokenInstance == "#")
        {
            return true;
        }
    }
    return false; //non-terminal nodes A and C do not have a varaible declared
}

// checking if any other nodes in the tree have a variable declared
bool varDeclared(node* nodeCheck)
{
    if (nodeCheck->tokenType == "D" || nodeCheck->tokenType == "G")
    {
        if (nodeCheck->child.size() >= 1) // if the non-terminals D or G have 1 or more child nodes
        {
            if (nodeCheck->tokenType == "D" &&  nodeCheck->child[0]->tokenType == "t1" && nodeCheck->child[0]->tokenInstance == "$")
                return true;
            if (nodeCheck->tokenType == "G" &&  nodeCheck->child[0]->tokenType == "t2")
                return true;
        }
    }
    return false; // there was no varaiables in the other nodes
}

void treeTraversing(node* currentNode, SymbolTable& table)
{
    if (currentNode == nullptr)
    {
        return;
    }
    if (nonTermCreated(currentNode)) // node contains a varaible
    {
        for (size_t index = 0; index < currentNode->child.size(); index++)
        {
            node* child = currentNode->child[index];

            if (child->tokenType == "t2")
            {
                // check if inserted into table
                if (!table.insert(child->tokenInstance))
                {
                    cout << "Error: " << child->tokenInstance << "is already inserted into table" << endl;
                    exit(1);
                }
            }
        }
    }
    else if (varDeclared(currentNode)) // seeing if there is a varibale in node
    {
        for (size_t index2 = 0; index2 < currentNode->child.size(); index2++)
        {
            node* child2 = currentNode->child[index2];

            if (child2->tokenType == "t2")
            {
                // checking if variable is already in table
                if (!table.verify(child2->tokenInstance))
                {
                    cout << "Error: " << child2->tokenInstance << " is not in the table" << endl;
                    exit(1);
                }
            }
        }
    }
    // recursive call
    for (size_t index3 = 0; index3 < currentNode->child.size(); index3++)
    {
        node* c = currentNode->child[index3];
        treeTraversing(c, table);
    }
}

void SymbolTable::checkTable(node* currentNode)
{
    treeTraversing(currentNode, *this);
}

void SymbolTable::printTable()
{
    cout << "Table is: " << endl;
    
    for (const string& symbol : symbol)
    {
        cout << symbol << endl;
    }
}