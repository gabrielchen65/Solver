#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <map>
#include "tokenizer.h"

class TreeNode;
class Tokenizer;

// Parser for the Boolean Formula
class FormulaParser {
    Tokenizer *tknzr;
    TreeNode *root;
    int LP = 0;
    TreeNode *parseFormula(); // parse non-terminal Formula in the grammar
    TreeNode *parseConjTerm(); // parse non-terminal ConjTerm in the grammar
    TreeNode *parseTerm(); // parse non-terminal Term in the grammar
    bool debugFlag = false;
  public:
    FormulaParser(string ln);
    TreeNode *getTreeRoot(); // parse the Boolean formula and return the root node of the formula syntax tree
    ~FormulaParser();
};

// Parser for the Assignment
class AssignmentParser {
    Tokenizer *tknzr;
    map<string, bool> results;
    string ln;
  public:
    AssignmentParser(string ln);
    map<string, bool> parseAssignment();
    ~AssignmentParser();
};

class LineHandler {
    string formularStr;
    string assignmentStr;
    string inLine;
  public:
    LineHandler(string ln):inLine(ln){};
    void splitLine();
    string getFormulaStr();
    string getAssignmentStr();
};

#endif
