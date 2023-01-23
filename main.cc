#include "tseitinTransformer.h"
#include "dpllsatSolver.h"
#include "parser.h"
#include "treeNode.h"
#include "exceptionHandler.h"
#include <iostream>
#include <string>
using namespace std;

// The program shall continuously ask for new inputs from standard input and output to the standard output
// The program should terminate gracefully (and quietly) once it sees EOF
int main() {
  while (true){ // continuously asking for new inputs from standard input
    std::string line; // store each input line
    std::getline(std::cin, line);
    if (std::cin.eof()) break; // terminate gracefully encountering eof
    try{
      FormulaParser fParser{line};
      TreeNode *treeRoot = nullptr;
      treeRoot = fParser.getTreeRoot();
      TseitinTransformer TT(treeRoot);
      std::vector<std::vector<int>> cnf = TT.transform();
      DPLLSatSolver dpllsolver{};
      std::map<std::string, bool> assignmentTable;
      if (dpllsolver.solver(cnf, assignmentTable)) std::cout << "sat" << std::endl;
      else std::cout << "unsat" << std::endl;
    }
    catch(parserException &ecpmsg){
      std::cout << ecpmsg.what() << std::endl;
      continue;
    }
  }
}
