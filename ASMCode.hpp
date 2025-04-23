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
        std::vector<std::string> usedVariables;
        std::ostream output;

        // processing token strings
        int processTokenThree(const std::string& token);
        std::string processTokenTwo(const std::string& token);

        // adding temp variables
        std::string createTempVariables();
    public:
        ASMCode(const std::string file, SymbolTable& symbolTable);

        void generateASM(node* node);
        void writeToASM();
};

#endif