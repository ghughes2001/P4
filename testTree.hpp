/*
Author: Grant Hughes
Created: March 26, 2025

testTree.hpp:
    - Functions for testing and printing the parse tree
*/

#ifndef _TEST_TREE_HPP_
#define _TEST_TREE_HPP_

#include "node.hpp"

void printTree(node* root, int indent);
void deleteTree(node* root);

#endif