/*
Author: Grant Hughes
Date: April 22, 2025

ASMCode.cpp:
    - defining file for generating assembly code
*/

#ifndef _ASM_CODE_HPP_
#define _ASM_CODE_HPP_

#include <fstream>
#include <string>
#include <vector>

#include "node.hpp"
#include "semantics.hpp"

class ASMCode
{
    private:
        int countForTempVariables;
        SymbolTable& table;
        std::vector<std::string> usedVariables; // store all used varaibles
        std::ofstream output;

        // processing token strings
        int processTokenThree(const std::string& token);
        std::string processTokenTwo(const std::string& token);

        // processing non-terminals in grammar
        void generateA(node* node);
        void generateB(node* node);
        void generateC(node* node);
        void generateD(node* node);
        void generateE(node*node);
        void generateF(node* node, std::string& variable);
        void generateG(node* node);
        void processEachNodeInTree(node* node);

        // adding temp variables
        std::string createExtraTempVariables();
    public:
        ASMCode(const std::string file, SymbolTable& symbolTable);
        void generateASM(node* node);
        void writeToASM();
};

#endif