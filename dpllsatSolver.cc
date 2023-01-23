#include "dpllsatSolver.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>

DPLLSatSolver::DPLLSatSolver(){}

// recursively call self to solve
bool DPLLSatSolver::solver(std::vector<std::vector<int>> cnf, std::map<std::string, bool> assignmentTable){
  // call bcp
  cnf = BCP(cnf, assignmentTable);
  // check sat, unsat
  int satStatus = checkSatStatus(cnf); // 1: SAT, 0: UNSAT, -1: No conclusion yet
  if (satStatus==1) return true; 
  else if (satStatus==0) return false;
  else{
    // choose variable
    std::string newVariable = chooseVar(cnf, assignmentTable);
    assignmentTable[newVariable] = true;
    if (solver(cnf, assignmentTable)){ 
      return true;
    }
    else {
      assignmentTable[newVariable] = false;
      if (solver(cnf, assignmentTable)){
        return true;
      }
      else{
        return false;
      }
    }
  }
}

std::vector<std::vector<int>> DPLLSatSolver::BCP(std::vector<std::vector<int>> cnf, std::map<std::string, bool> assignmentTable){
  std::string mapKey;
  bool mapValue;
  int curlit;
  std::vector<int> dropClauseList;
  std::vector<int> dropLitList;
  // for each assignment variable
  for (auto it = assignmentTable.cbegin(); it != assignmentTable.cend(); it++){
    mapKey = it->first; mapValue = it->second;
    dropClauseList.clear();
    // for each clause
    for (int i=0;i!=(int)cnf.size();i++){
      dropLitList.clear();
      // for each literal in clause
      for (int j=0;j!=(int)cnf[i].size();j++){
        curlit = (int)cnf[i][j];
        if (mapKey != std::to_string(abs(curlit))) continue;
        if ((curlit>0 && mapValue) || (curlit<0 && !mapValue)){
          // drop the clause
          dropClauseList.push_back(i);
          break;
        }
        if ((curlit>0 && !mapValue) || (curlit<0 && mapValue)){
          // drop the literal
          dropLitList.push_back(j);
        }
      }
      // drop literals for current clause
      if ((int)cnf.size()>0 && (int)dropLitList.size()>0){
        cnf[i] = dropLit(cnf[i], dropLitList, assignmentTable);
      }
      // if current clause becomes unit clause, drop clause and save it in map
      if (cnf[i].size() == 1){
        int literal = cnf[i][0];
        std::string leftKey = std::to_string(abs(cnf[i][0]));
        if (assignmentTable.find(leftKey)!=assignmentTable.end()){
          // terminate unit assigning if conflict happened
          if (assignmentTable[leftKey] && literal<0){
            cnf[i].pop_back();
          }
        }
        else{
          if ((int)literal > 0) assignmentTable[leftKey] = true;
          else assignmentTable[leftKey] = false;
        }
      }
    }
    if ((int)cnf.size()>0 && (int)dropClauseList.size()>0){
      cnf = dropClause(cnf, dropClauseList, assignmentTable);
    }
  }
  return cnf;
}

std::vector<std::vector<int>> DPLLSatSolver::PLP(std::vector<std::vector<int>> cnf){
  return cnf;
}

std::string DPLLSatSolver::chooseVar(std::vector<std::vector<int>> cnf, std::map<std::string, bool> assignmentTable){
  std::string cur;
  if ((int) cnf.size() != 0){
    for (int i=0;i!=(int)cnf.size();i++){
      for (int j=0;j!=(int)cnf[i].size();j++){
        cur = std::to_string(abs(cnf[i][j]));
        if (assignmentTable.count(cur) == 0) return cur;
      }
    }
  }
  return "";
}

int DPLLSatSolver::checkSatStatus(std::vector<std::vector<int>> cnf){
  if ((int)cnf.size()==0) return 1;
  else{
    for (int i=0;i!=(int)cnf.size();i++){
      // if empty clause, return UNSAT; but empty formula is SAT
      if ((int)cnf[i].size()==0) return 0;
    }
    return -1;
  }
}


void DPLLSatSolver::printMap(std::map<std::string, bool> assignmentTable){
  // print assignment map 
  for (auto it = assignmentTable.cbegin(); it != assignmentTable.cend(); it++){
    std::cout << it->first << ": " << it->second << ", ";
  }
  std::cout << std::endl;
}

std::string DPLLSatSolver::cnfString(std::vector<std::vector<int>> cnf){
  std::string result = "";
  std::cout << "[ ";
  for (long unsigned int i = 0; i!=cnf.size(); i++){
    std::cout << "[";
    for(long unsigned int j = 0; j!=cnf[i].size(); j++){
      std::cout << cnf[i][j] << ", ";
    }
    std::cout << "], ";
  }
  std::cout << "]" << std::endl;
  return result;
}

std::string DPLLSatSolver::cnfString(std::vector<int> cnf){
  std::string result = "";
  std::cout << "[ ";
  for (long unsigned int i = 0; i!=cnf.size(); i++){
      std::cout << cnf[i] << ", ";
  }
  std::cout << "]" << std::endl;
  return result;
}

// for dropping clauses
std::vector<std::vector<int>> DPLLSatSolver::dropClause(std::vector<std::vector<int>> cnf, std::vector<int> dlist, std::map<std::string, bool> assignmentTable){
  //cnf[i].erase(cnf[i].begin() + j);
  if ((int)cnf.size()==0) return cnf;
  int targetIndex;
  for (int i=(int)dlist.size()-1;i!=-1;i--){
    targetIndex = dlist[i];
    cnf[targetIndex] = cnf.back();
    cnf.pop_back();
  }
  return cnf;
}

// for dropping literals
std::vector<int> DPLLSatSolver::dropLit(std::vector<int> cnf, std::vector<int> dlist, std::map<std::string, bool> assignmentTable){
  if ((int)cnf.size()==0) return cnf;
  int targetIndex;
  for (int i=(int)dlist.size()-1;i!=-1;i--){
    targetIndex = dlist[i];
    cnf[targetIndex] = cnf.back();
    cnf.pop_back();
  }
  return cnf;
}
