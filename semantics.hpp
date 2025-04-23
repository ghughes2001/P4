/*
Auhtor: Grant Hughes
Date: April 13, 2025

semantics.hpp
    - header file for semantics class (needs to be more private than the other parts of project)
*/

#ifndef _SEMANTICS_HPP_
#define _SEMANTICS_HPP_

#include <vector>
#include <string>
#include "node.hpp"

class SymbolTable {
    private:
        std::vector<std::string> symbol;
    public:
        bool verify(std::string var); // method check if string is in table
        bool insert(std::string var); // method to insert into table
        void printTable(); // method to print the table after ALL operations are completed
        void checkTable(node* currentNode); // check the semantics in table
};


#endif