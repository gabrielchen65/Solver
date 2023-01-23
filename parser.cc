#include "parser.h"
#include "treeNode.h"
#include "tokenizer.h"
#include "exceptionHandler.h"
using namespace std;

FormulaParser::FormulaParser(string ln): tknzr {new Tokenizer{ln}} {}

TreeNode *FormulaParser::parseFormula() {
  if (debugFlag) cout << "parseFormula." << endl;
  if (!tknzr->hasToken()){ 
    //throw parserException("(begining of parseFormula) Error: invalid input");
    throw parserException("Error: invalid input");
  }
  TreeNode *node1 = parseConjTerm();
  if (tknzr->hasToken()){
    string tokenContent = tknzr->getToken().content;
    string tokenType = tknzr->getToken().type;
    //cout << "(parseFormula) type, content: " << tokenType << ", " << tokenContent << endl; 
    if (tokenContent == ")"){
      return node1;
    }
    else if (tokenContent == "+"){
      // cout << "(parseFoumula) content: " << tokenContent << endl;
      TreeNode* parent = new TreeNode(tokenType, tokenContent);
      tknzr->advanceToken();
      TreeNode* node2 = parseFormula();
      parent->updateChildren(node1, node2);

      //cout << "Parse Formula" << endl;
      //debugPrint(parent); debugPrint(node1); debugPrint(node2);
      return parent;
    }
    else{
      //cout << "tk content: " << tokenContent << endl;
      //throw parserException("(end of parseFormula) Error: invalid input");
      throw parserException("Error: invalid input");
    }
  }
  return node1;
}

TreeNode *FormulaParser::parseConjTerm() {
  if (debugFlag) cout << "parseConjTerm." << endl;
  if (!tknzr->hasToken()) {
    //throw parserException("(Begining of parseConjTerm) Error: invalid input");
    throw parserException("Error: invalud input");
  }
  TreeNode *node1 = parseTerm();
  if (tknzr->hasToken()){
    string tokenContent = tknzr->getToken().content;
    string tokenType = tknzr->getToken().type;
    if (tokenContent == ")" || tokenContent == "+"){
      return node1;
    }
    else if (tokenContent == "*"){
      //cout << "(parseConjTerm) content: " << tokenContent << endl;
      TreeNode* parent = new TreeNode(tokenType, tokenContent);
      tknzr->advanceToken();
      TreeNode* node2 = parseConjTerm();
      parent->updateChildren(node1, node2);

      //cout << "Parse ConjTerm" << endl;
      //debugPrint(parent); debugPrint(node1); debugPrint(node2);
      return parent;
    }
    else {
      if (debugFlag) cout << "tk content:" << tokenContent << "(end)" << endl;
      //throw parserException("(end of parseConjTerm) Error: invalid input");
      throw parserException("Error: invalid input");
    }
  }
  return node1;
}

TreeNode *FormulaParser::parseTerm() {
  if (debugFlag) cout << "parseTerm." << endl;
  if (!tknzr->hasToken()) {
    //throw parserException("(Begining of parseTerm) Error: invalid input");
    throw parserException("Error: invalid input");
  }
  string tokenType = tknzr->getToken().type;
  string tokenContent = tknzr->getToken().content;
  if (debugFlag) cout << "current content:" << tokenContent << "(end)" << endl;
  if (tokenType == "constant"){
    TreeNode* node = new TreeNode(tokenType, tokenContent);
    tknzr->advanceToken();
    return node;
  } 
  else if(tokenType == "variable"){
    // assign value
    TreeNode* node = new TreeNode(tokenType, tokenContent);
    tknzr->advanceToken();
    //cout << "(parser, formula) token type, content: " << tokenType << ", " << tokenContent << endl;
    return node;
  }
  // else for "operator"
  else if (tokenContent == "("){
    // LP += 1;      
    //cout << "LP, LP point: " << LP << endl;
    tknzr->advanceToken();
    TreeNode *node = parseFormula();
    if (!tknzr->hasToken()) {
      //throw parserException("(next term of left parenthesis) Error: invalid input");
      throw parserException("Error: invalid input");
    }
    if (tknzr->getToken().content != ")"){
      //throw parserException("(no ')' ) Error: invalid input");
      throw parserException("Error: invalid input");
    }
    //else{ LP -= 1;}
    tknzr->advanceToken();
    return node;
  }
  else if (tokenContent == "-"){
    TreeNode* parent = new TreeNode(tokenType, tokenContent);
    tknzr->advanceToken();
    TreeNode* node = parseTerm();
    parent->updateLeftChild(node);
    return parent;
  }
  else {
    if (debugFlag) cout << "token content:" << tokenContent << "(end)" << endl;
    // throw parserException("(End of parseTerm) Error: invalid input");
    throw parserException("Error: invalid input");
  }
}

TreeNode * FormulaParser::getTreeRoot() {
  if (tknzr->hasToken()){
    Token currentToken = tknzr->getToken();
    if (currentToken.content[0] == ' ') tknzr->advanceToken();
  }
  TreeNode* node = parseFormula();
  //cout << "last token: " << tknzr->getToken().content << endl;
  if (tknzr->hasToken()){
    //throw parserException("(getTreeRoot) Error: invalid input");
    throw parserException("Error: invalid input");
  }
  return node;
}


FormulaParser::~FormulaParser() {
}


AssignmentParser::AssignmentParser(string ln): tknzr {new Tokenizer{ln}} {}


map<string, bool> AssignmentParser::parseAssignment() {
  string curVariable;
  //bool curConstant;
  // check and skip prefixed spaces
  if (tknzr->hasToken()){
    Token initToken = tknzr->getToken();
    if (initToken.content[0] == ' ') tknzr->advanceToken();
  }
  while(tknzr->hasToken()){
    Token currentToken = tknzr->getToken();
    if (currentToken.type == "variable"){
      curVariable=currentToken.content;
      //cout << "(parser, assignment, Variable) token type, content: " << currentToken.type << ", " << curVariable << endl;
      tknzr->advanceToken();
      currentToken = tknzr->getToken();
      // check if ":" exists
      if (currentToken.content != ":"){
        //throw parserException("(no :) Error: invalid input");
        throw parserException("Error: invalid input");
      }
      tknzr->advanceToken();
      currentToken = tknzr->getToken();
      //cout << "currentToken: " << currentToken.content << endl;
      if (currentToken.type == "constant"){
        //cout << "In assignment parser, constant" << endl;
        bool truthValue = stoi((tknzr->getToken()).content);
        if (results.find(curVariable) == results.end()){
          results[curVariable] = truthValue;
        }
        else{
          if (results[curVariable] != truthValue){
            throw parserException("Error: contradicting assignment");
          }
        }
        //cout << "(parser, assignment, Var and Const) var, cons: " << curVariable << ", " << curConstant << endl;
        if (!tknzr->advanceToken()) break;
        currentToken = tknzr->getToken();
        if (currentToken.content != ","){
          throw parserException("Error: invalid input");
        }
      }
      else {
        //throw parserException("(not constant) Error: invalid input");
        throw parserException("Error: invalid input");
      }
    }
    else {
      //throw parserException("(not variable) Error: invalid input");
      throw parserException("Error: invalid input");
    }
    if (!tknzr->advanceToken()) break;
  }

  /*map<string, bool>::iterator it;
  for (it=results.begin(); it!=results.end(); ++it){
    cout << "results Var: " << it->first << " result Cons: " << it->second << endl;
  }*/
  return results;
}

AssignmentParser::~AssignmentParser() {
  // your code starts here
}


void LineHandler::splitLine(){
  string delimiter = ";";
  if (inLine.find(delimiter) == std::string::npos){
    //throw parserException("(no ;) Error: invalid input");
    throw parserException("Error: invalid input");
  }
  formularStr = inLine.substr(0, inLine.find(delimiter));
  assignmentStr = inLine.substr(inLine.find(delimiter)+1, inLine.length());
  //cout << "formular: " << formularStr << endl;
  //cout << "assignment: " << assignmentStr << endl;
}

string LineHandler::getFormulaStr(){ return formularStr; }

string LineHandler::getAssignmentStr(){ return assignmentStr; }
