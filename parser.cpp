/*
Author: Grant Hughes
Created: March 23, 2025

parser.cpp:
    - Implamentation of parser interface for BNF grammar
*/

#include "token.hpp"
#include "node.hpp"
#include "scanner.hpp"
#include "parser.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


// global varaibels to track tokens
static vector<Token> tokens;
static size_t tokenSize = 0;

// prototypes
node* S();
node* A();
node* B();
node* C();
node* D();
node* E();
node* F();
node* G();

// helper function to return current token
Token& currentToken()
{
    if (tokenSize < tokens.size())
    {
        return tokens[tokenSize];
    }
    // otherwise retunr EOF
    static Token EOFToken(EOFTk, "EOF", -1);
    return EOFToken;
}

// helper fucntion to return errors in grammer
void parsingError(const string& expected) {
    Token current = currentToken();
    cout << "PARSER ERROR: Expected " << expected << ", got '" 
         << current.tokenInstance << "' on line " << current.lineNumber << endl;
    exit(1);
}

// return the next token
void nextToken()
{
    if (tokenSize < tokens.size())
    {
        tokenSize++;
    }
}

// S -> A(BB)
node* S()
{
    node* nodeForS = new node("S");  // creating non-terminal S(root)
    nodeForS->addChildren(A());      // creating A (non-terminal)(node)
    
    if (currentToken().tokenInstance == "(")
    {
        node* leftParenthesis = new node("t1", "(");
        nodeForS->addChildren(leftParenthesis);
        nextToken();
    }
    else
    {
        parsingError("( in S non-terminal grammar");
    }
    
    // Adding the first B non-terminal
    nodeForS->addChildren(B());
    
    // Check if we need to process second B or if we're already at closing parenthesis
    if (currentToken().tokenInstance != ")")
    {
        nodeForS->addChildren(B());
    }
    else
    {
        // Add an empty B node
        node* emptyB = new node("B");
        node* empty = new node("EMPTY");
        emptyB->addChildren(empty);
        nodeForS->addChildren(emptyB);
    }
    
    // Handling the end parenthesis
    if (currentToken().tokenInstance == ")")
    {
        node* rightParenthesis = new node("t1", ")");
        nodeForS->addChildren(rightParenthesis);
        nextToken();
    }
    else
    {
        parsingError("Expected ) at end of S production");
    }
    
    return nodeForS;
}

node* A()
{
    node* nodeForA = new node("A"); //creating non-terminal A

    if (currentToken().tokenInstance == "\"")
    {
        node* quote = new node("t1", "\"");
        nodeForA->addChildren(quote); // add node for "
        nextToken(); // move to next token

        // now at t2 since we had a "
        if (currentToken().tokenID == t2_tk)
        {
            node* tokenTwo = new node("t2", currentToken().tokenInstance);
            nodeForA->addChildren(tokenTwo); // add node for t2
            nextToken();
        }
        else
        {
            // missing t2 token so error in tree
            parsingError("Needs t2_tk for non-terminal A grammar");
        }
    }
    else
    {
        node* empty = new node("EMPTY");
        nodeForA->addChildren(empty); // add the empty node as child
    }
    // return either (" t2) or (empty) node
    return nodeForA;
}

node* B()
{
    node* nodeForB = new node("B");  // creating non-terminal B node
    Token current = currentToken();  // getting current token

    // B can derive S, C, D, E, or G - need to check each possibility
    
    // B -> S
    if (current.tokenInstance == "\"" || current.tokenInstance == "(")
    {
        nodeForB->addChildren(S());
        return nodeForB;
    }
    // B -> C
    else if (current.tokenInstance == "#" || current.tokenInstance == "!")
    {
        nodeForB->addChildren(C());
        return nodeForB;
    }
    // B -> D
    else if (current.tokenInstance == "$")
    {
        nodeForB->addChildren(D());
        return nodeForB;
    }
    // B -> E
    else if (current.tokenInstance == "'")
    {
        nodeForB->addChildren(E());
        return nodeForB;
    }
    // B -> G
    else if (current.tokenID == t2_tk)
    {
        nodeForB->addChildren(G());
        return nodeForB;
    }
    else if (current.tokenInstance == ")")
    {
        // B can be empty - add an EMPTY node
        node* empty = new node("EMPTY");
        nodeForB->addChildren(empty);
        return nodeForB;
    }
    else
    {
        // If none of the production rules match
        parsingError("Valid start of B production (one of: \", (, #, !, $, ', or t2 token)");
    }
    
    return nodeForB;  // This line will never be reached due to parsingError
}

node* C()
{
    node* nodeForC = new node("C"); // creating C non-terminal node

    if (currentToken().tokenInstance == "#") {
        node* hashNumber = new node("t1", "#");
        nodeForC->addChildren(hashNumber);
        nextToken();
        
        // next token is t2
        if (currentToken().tokenID == t2_tk) {
            node* t2 = new node("t2", currentToken().tokenInstance);
            nodeForC->addChildren(t2);
            nextToken();
        } else {
            parsingError("Need t2_tk");
        }
    }
    else if (currentToken().tokenInstance == "!") {
        node* exclamation = new node("t1", "!");
        nodeForC->addChildren(exclamation);
        nextToken();
        
        // Parse F
        nodeForC->addChildren(F());
    } else {
        parsingError("Need '#' or '!' for C non-terminal grammar");
    }
    // returnng one of the temrinals
    return nodeForC;
}

node* D()
{
    node* nodeForD = new node("D"); // creating D non-terminal node

    if (currentToken().tokenInstance == "$")
    {
        node* dollar = new node("t1", "$"); // creating dollar node
        nodeForD->addChildren(dollar); // adding dollar node
        nextToken();

        // adding/creating F non-temrinal node
        nodeForD->addChildren(F());
    }
    else
    {
        parsingError("Need $ for D non-terminal grammar");
    }
    // returning the result
    return nodeForD;
}

node* E()
{
    node* nodeForE = new node("E"); // adding non-temrinal node E

    if (currentToken().tokenInstance == "'")
    {
        node* quote = new node("t1", "'"); // creating node for quote
        nodeForE->addChildren(quote); // adding quote as child for E
        nextToken();

        // adding the F's non-terminal node
        nodeForE->addChildren(F()); // first E
        nodeForE->addChildren(F()); // second E
        nodeForE->addChildren(F()); // third E
        // adding the B non-temrinal node
        nodeForE->addChildren(B());
    }
    else
    {
        parsingError("Need ' for E non-terminal grammar");
    }
    // return result
    return nodeForE;
}

node* F()
{
    node* nodeForF = new node("F");
    
    // Check for t2 token
    if (currentToken().tokenID == t2_tk)
    {
        node* tokenTwo = new node("t2", currentToken().tokenInstance);
        nodeForF->addChildren(tokenTwo);
        nextToken();
        return nodeForF;  // Return after processing t2
    }
    // Check for t3 token
    else if (currentToken().tokenID == t3_tk)
    {
        node* tokenThree = new node("t3", currentToken().tokenInstance);
        nodeForF->addChildren(tokenThree);
        nextToken();
        return nodeForF;  // Return after processing t3
    }
    // Check for & symbol
    else if (currentToken().tokenInstance == "&")
    {
        node* andSymbol = new node("t1", "&");
        nodeForF->addChildren(andSymbol);
        nextToken();
        
        // now the F's
        nodeForF->addChildren(F());
        nodeForF->addChildren(F());
        return nodeForF;
    }
    else
    {
        parsingError("Need t2_tk, t3_tk, or & for F non-termial grammar");
    }
    
    return nodeForF;
}

node* G()
{
    node* nodeForG = new node("G");

    if (currentToken().tokenID != t2_tk)
    {
        parsingError("Need t2_tk for G non-terminal grammar");
    }
    
    node* tokenOfTwo = new node("t2", currentToken().tokenInstance); // creating token 2 as node
    nodeForG->addChildren(tokenOfTwo); // adding token 2 as child
    nextToken();

    // now the %
    if (currentToken().tokenInstance != "%")
    {
        parsingError("Need % for G non-terminal grammar");
    }
    
    node* percent = new node("t1", "%"); // creating node for %
    nodeForG->addChildren(percent); // adding node as child to G
    nextToken();

    // parsing F
    nodeForG->addChildren(F());

    // return result in G
    return nodeForG;
}

node* parser(const string &fileName)
{
    // opening file in read
    ifstream file(fileName);
    tokens = scanner(file, fileName, 0);
    tokenSize = 0;

    // creating the root of tree (start of grammar)
    node* root = S();

    if (currentToken().tokenID != EOFTk)
    {
        parsingError("End of file");
    }
    return root;
}