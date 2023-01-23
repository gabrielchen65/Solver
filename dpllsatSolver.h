#ifndef SAT_H
#define SAT_H

#include <vector>
#include "treeNode.h"

class DPLLSatSolver{
  public:
    DPLLSatSolver();
    bool solver(std::vector<std::vector<int>> cnf, std::map<std::string, bool> assignmentTable);
    std::vector<std::vector<int>> BCP(std::vector<std::vector<int>> cnf, std::map<std::string, bool> assignmentTable);
    std::vector<std::vector<int>> PLP(std::vector<std::vector<int>> cnf);
    std::string chooseVar(std::vector<std::vector<int>> cnf, std::map<std::string, bool> assignmentTable);
    int checkSatStatus(std::vector<std::vector<int>> cnf);
    void printMap(std::map<std::string, bool> assignmentTable);
    std::vector<std::vector<int>> dropClause(std::vector<std::vector<int>> cnf, std::vector<int> dlist, std::map<std::string, bool> assignmentTable);
    std::vector<int> dropLit(std::vector<int> cnf, std::vector<int> dlist, std::map<std::string, bool> assignmentTable);
    std::string cnfString(std::vector<std::vector<int>> cnf);
    std::string cnfString(std::vector<int> cnf);
};
#endif
