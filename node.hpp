/*
Author: Grant Hughes
Created: March 16, 2025

node.hpp:
    - Node structure for parse tree
*/

#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <string>
#include <vector>
#include "token.hpp"

struct node {
    std::string tokenType;
    std::string tokenInstance;
    std::vector<node*> child;

    // Constructor for non-terminal node
    node(const std::string &nodeType) : tokenType(nodeType), tokenInstance("") {}
    
    // Constructor for terminal node (with token)
    node(const std::string &nodeType, const std::string &nodeInstance) : tokenType(nodeType), tokenInstance(nodeInstance) {}

    void addChildren(node* c)
    {
        if (c != nullptr)
        {
            child.push_back(c);
        }
    }
};

#endif