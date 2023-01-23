#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <regex>
#include <iostream>
using namespace std;
typedef vector<string> StringVector;

struct Token
{
    string content;
    string type;
};

// a tokenzier breaks a stream of string into tokens
class Tokenizer
{
  // your code starts here
  private:  
    StringVector v;
    StringVector::iterator iter;
    Token currentToken;
  public:
    Tokenizer(string ln);
    int getVectorLength();
    bool advanceToken(); // advance to the next token; return false if no more token exist
    bool hasToken() const; // check whether there is a current token
    Token getToken(); // returns the token being processed
};

#endif
