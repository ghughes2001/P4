#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype> // isalnum, isalpha, isdigit
#include <sstream> // istringstream

#include "token.hpp"
#include "scanner.hpp"

using namespace std;

// checking if file is empty
bool isFileEmpty(const string &filename) {
    std::ifstream file(filename);
    return file.peek() == std::ifstream::traits_type::eof();
}

// function to remove comments
string removeComments(string str)
{
    size_t startOfComment = str.find('*');
    while (startOfComment != string::npos) // if another '*' is found
    {
        size_t endOfComment = str.find('*', startOfComment + 1);
        if (endOfComment == string::npos) // there is no other '*'
            break;
        return str.substr(0, startOfComment) + str.substr(endOfComment + 1); // removing the comment and everything in-between
    }
    return str;
}

// checking if token1
bool tokenOneCheck(string str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        char c = str[i];
        if (c == '!' || c == '"' || c == '#' || c == '$' || c == '%' || c == '&' 
            || c == '(' || c == ')' || c == '\'' )
            continue;
        if (isspace(c))
            continue;
        return false; // invalid
    }
    return true; // valid
}

// checking if token2 (numbers starting with a +)
bool tokenTwoCheck(string str)
{
    if (str.empty() || str[0] != '+')
        return false;
    for (size_t i = 1; i < str.length(); i++)
    {
        if (!isdigit(str[i])) // checking if the rest are digits
            return false;
    }
    return true;
}

// checking if token3 (letters followed by digits)
bool tokenThreeCheck(string str)
{
    if (str.empty() || !isalpha(str[0]))
        return false;
    for (size_t i = 1; i < str.length(); i++)
    {
        if (!isdigit(str[i])) // checking if the rest are digits
            return false;
    }
    return true;
}

// Function that splits merged tokens/checks token
vector<string> splitTokens(string str) {
    vector<string> tokens;
    string currentToken;

    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        
        // Skip whitespace, but finalize any current token
        if (isspace(c)) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            continue;
        }
        
        // Handle special characters (token type t1)
        if (c == '!' || c == '"' || c == '#' || c == '$' || c == '%' || 
            c == '&' || c == '\'' || c == '(' || c == ')') {
            // Save any pending token before adding the special character
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            // Add the special character as its own token
            tokens.push_back(string(1, c));
            continue;
        }
        
        // Handle '+' character (potential start of token type t2)
        if (c == '+') {
            // If we already have a token started, finalize it
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            currentToken += c;
            continue;
        }
        
        // Handle letters (potential start of token type t3)
        if (isalpha(c)) {
            // If current token is empty or is a '+' (which shouldn't be followed by a letter)
            // or if current token ends with a digit (need to start a new token)
            if (currentToken.empty() || 
                (currentToken.length() == 1 && currentToken[0] == '+') ||
                (!currentToken.empty() && isdigit(currentToken.back()))) {
                
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
            }
            currentToken += c;
            continue;
        }
        
        // Handle digits
        if (isdigit(c)) {
            // If current token is empty, or it ends with a letter and it's not just one letter
            // (which would be the start of a t3 token), start a new token
            if (currentToken.empty() || 
                (!currentToken.empty() && isalpha(currentToken.back()) && currentToken.length() > 1)) {
                
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
            }
            currentToken += c;
            continue;
        }
        
        // For any other character, report scanner error
        cout << "SCANNER ERROR: " << c << ", " << str << endl;
        exit(1);
    }
    
    // Don't forget the last token
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }
    
    return tokens;
}

vector<Token> scanner(ifstream &inputFile, const string &filename, int startLine = 1)
{
    vector<Token> tokens;
    string lineContent;
    int line = startLine;

    if (isFileEmpty(filename))
    {
        cout << "Error: File missing data" << endl;
        exit(1);
    }
    if (inputFile.is_open())
    {
        while (getline(inputFile, lineContent))
        {
            line++; // track line number
            lineContent = removeComments(lineContent); // removing comments
            vector<string> words = splitTokens(lineContent);

            for (size_t i = 0; i < words.size(); i++) // process each word
            {
                string word = words[i];
                Token t(EOFTk, "EOF", line);

                // check tokens
                if (word.length() == 1)
                {
                    if (!tokenOneCheck(word))
                    {
                        cout << "SCANNER ERROR: " << word << ", " << line << endl;
                        exit(1);
                    }
                    t = Token(t1_tk, word, line);
                }
                else if (word[0] == '+' && word.length() > 1)
                {
                    if (!tokenTwoCheck(word))
                    {
                        cout << "SCANNER ERROR: " << word << ", " << line << endl;
                        exit(1);
                    }
                    t = Token(t2_tk, word, line);
                }
                else if (isalpha(word[0]) && word.length() > 1)
                {
                    if (!tokenThreeCheck(word))
                    {
                        cout << "SCANNER ERROR: " << word << ", " << line << endl;
                        exit(1);
                    }
                    t = Token(t3_tk, word, line);
                }
                if (t.tokenID != EOFTk)
                {
                    tokens.push_back(t);
                }
            }
        }

        // Add the EOF token after reading all lines
        tokens.push_back(Token(EOFTk, "EOF", line));

        inputFile.clear();
        inputFile.seekg(0, ios::beg);
        inputFile.close();
    }
    else
    {
        cout << filename << " couldn't open" << endl;
        exit(1);
    }

    return tokens;
}