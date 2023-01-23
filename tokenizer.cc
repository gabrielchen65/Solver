#include "tokenizer.h"
#include "exceptionHandler.h"
#include <regex>
#include <iostream>
#include <string>
using namespace std;


Tokenizer::Tokenizer(std::string line) {
  regex re("[a-zA-Z0-9]+|[\\s*+-;:,\\(\\)]|[^\\s*+-;:,\\(\\)]+");
  sregex_token_iterator begin(line.begin(),line.end(),re), end;
  copy(begin, end, back_inserter(v));
  iter = v.begin();
  // handle zero length assignment
  if (line.length() == 0){ iter = v.end(); }
  //if (hasToken()){currentToken = getToken();}
}

int Tokenizer::getVectorLength(){
  return v.size();
}

bool Tokenizer::advanceToken() {
  Token curContent;
  while (iter+1 < v.end()) {
    iter++;
    curContent.content = *iter;
    if (curContent.content[0] == ' ') continue;
    return true;
  }
  iter++;
  return false;
}

bool Tokenizer::hasToken() const {
  return (iter) < v.end();
}


// only can be called when hasToken() is true
Token Tokenizer::getToken() {
  currentToken.content = *iter;
  char firstL = currentToken.content[0];
  if (('a' <= firstL && firstL <= 'z') 
    || ('A' <= firstL && firstL <='Z')){
    //cout << "(var) firstL: " << firstL << endl;
    currentToken.type = "variable";
    //cout << "(Tokenizer) firstL: " << firstL;
    //cout << ", token type: " << currentToken.type;
    //cout << ", token content: " << currentToken.content << endl;
    if (currentToken.content.length() > 10) {
      throw parserException("Error: invalid input");
    }
  }
  else if (firstL == '*' || firstL == '+'
    || firstL == '(' || firstL == ')' 
    || firstL == '-' || firstL == ':'
    || firstL == ','){
    //cout << "(opt) firstL: " << firstL << endl;
    currentToken.type = "operator";
  }
  else if (firstL == '1' || firstL == '0'){
    //cout << "(const) firstL: " << firstL << endl;
    currentToken.type = "constant";
  }
  else if (firstL == ' '){
    currentToken.type = "space";
  }
  else{
    //cout << "firstL:" << firstL << "(END)" << endl;
    //throw parserException("(getToken) Error: invalid input");
    throw parserException("Error: invalid input");
  }
  //cout << "contend & type: " << currentToken.content 
  //    << "," << currentToken.type << endl;
  return currentToken;
}

