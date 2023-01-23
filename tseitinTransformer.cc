#include "tseitinTransformer.h"
#include "treeNode.h"
#include <vector>
#include <iostream>
using namespace std;

TseitinTransformer::TseitinTransformer(TreeNode *root): formulaRoot{root} {}

int TseitinTransformer::transSubformula(TreeNode* subRoot) {
  // advance iterator, recursively call
  int curID = varIdCounter;
  if (checkIfLeaf(subRoot)){
    //cout << "first case" << endl;
    return storeToTable(subRoot);
  }
  else if(subRoot->getContent() == "*"){
    varIdCounter++;
    //cout << "* case" << endl;
    int leftChild = transSubformula(subRoot->getLeftChild());
    int rightChild = transSubformula(subRoot->getRightChild());
    addAndEq(curID, leftChild, rightChild);
  }
  else if(subRoot->getContent() == "+"){
    varIdCounter++;
    //cout << "+ case" << endl;
    int leftChild = transSubformula(subRoot->getLeftChild());
    int rightChild = transSubformula(subRoot->getRightChild());
    addOrEq(curID, leftChild, rightChild);
  }
  else{ // for the case "-"
    varIdCounter++;   
    //cout << "- case" << endl;
    int leftChild = transSubformula(subRoot->getLeftChild());
    addNegEq(curID, leftChild);
  }
  return curID;
}

void TseitinTransformer::addNegEq(int curID, int childID) {
  // [-curID, -childID], [curID, childID]
  vector<int> v1 = {curID, childID};
  cnf.insert(cnf.begin(), v1);
  vector<int> v = {-curID, -childID};
  cnf.insert(cnf.begin(), v);
}

void TseitinTransformer::addOrEq(int curID, int leftID, int rightID) {
  // [-cur,l,r], [cur,-l], [cur,-r]
  vector<int> v2 = {curID, -rightID};
  cnf.insert(cnf.begin(), v2);
  vector<int> v1 = {curID, -leftID};
  cnf.insert(cnf.begin(), v1);
  vector<int> v = {-curID, leftID, rightID};
  cnf.insert(cnf.begin(), v);
}

void TseitinTransformer::addAndEq(int curID, int leftID, int rightID) {
  // [-cur,l], [-cur,r], [cur, -l, -r]
  vector<int> v2 = {curID, -leftID, -rightID};
  cnf.insert(cnf.begin(), v2);
  vector<int> v1 = {-curID, rightID};
  cnf.insert(cnf.begin(), v1);
  vector<int> v = {-curID, leftID};
  cnf.insert(cnf.begin(), v);
}

void TseitinTransformer::NNFConversion(TreeNode *root){
  // push neg, skip double negations
  /*TreeNode* leftChild = root->getLeftChild();
  TreeNode* rightChild = root->getRightChild();
  if (leftChild->getContent() == "-"){
    if (leftChild->getLeftChild()->getContent() == "-"){
      root->updateLeftChild( root->getLeftChild()->getLeftChild()->getLeftChild() );
    }
    else if (leftChild->getLeftChild()->getContent() == "*"){
      std::string s = "-";
      TreeNode* newLeft = new TreeNode(s);
      TreeNode* newRight = new TreeNode(s);
      TreeNode* oriLeft = leftChild->getLeftChild()->getLeftChild();
      TreeNode* oriRight = leftChild->getLeftChild()->getRightChild();
      leftChild->getLeftChild()->updateChildren(newLeft, newRight);
      leftChild->getLeftChild()->updateContent("+");
      leftChild->getLeftChild()->getLeftChild()->updateLeftChild(oriLeft); // LLL-> L
      leftChild->getLeftChild()->getRightChild()->updateLeftChild(oriRight); // LLR -> L
    }
    else if (leftChild->getContent() == "+"){
      TreeNode* newLeft(string s="-");
      TreeNode* newRight(string s="-");
      TreeNode* oriLeft = leftChild->getLeftChild()->getLeftChild();
      TreeNode* oriRight = leftChild->getLeftChild()->getRightChild();
      //leftChild->getLeftChild()->updateChildren(newLeft, newRight);
      leftChild->getLeftChild()->updateContent("*");
      leftChild->getLeftChild()->getLeftChild()->updateLeftChild(oriLeft); // LLL-> L
      leftChild->getLeftChild()->getRightChild()->updateLeftChild(oriRight); // LLR -> L
    }
    else{
      // if not variables, throw exception
      //throw "Error: ";
    }
  }
  if (root->getRightChild()->getContent() == "-"){
    if (rightChild->getLeftChild()->getContent() == "-"){
      root->updateLeftChild( root->getLeftChild()->getLeftChild()->getLeftChild() );
    }
    else if (rightChild->getContent() == "*"){
      TreeNode* nodeLeft(string s="-");
      TreeNode* nodeRight(string s="-");
      rightChild->updateChildren(nodeLeft, nodeRight);
      rightChild->updateContent("+");
    }
    else if (rightChild->getContent() == "+"){
      TreeNode* nodeLeft(string s="-");
      TreeNode* nodeRight(string s="-");
      rightChild->updateChildren(nodeLeft, nodeRight);
      rightChild->updateContent("*");
    }
    else{
      // if not variables, throw exception
      //throw "Error: ";
    }
  }
  NNFConversion(root->getLeftChild());
  NNFConversion(root->getRightChild());*/
}

std::vector<std::vector<int>> TseitinTransformer::transform() {
  // counter save F as 1 and advance
  //NNFConversion(formulaRoot);
  vector<int> v = {varIdCounter};
  cnf.insert(cnf.begin(), v);
  transSubformula(formulaRoot);
  return cnf;
}

std::string TseitinTransformer::cnfString() const {
  std::string result = "";
  //cout << "cnf size:" << cnf.size() << endl;
  cout << "[ ";
  for (long unsigned int i = 0; i!=cnf.size(); i++){
    cout << "[";
    for(long unsigned int j = 0; j!=cnf[i].size(); j++){
      cout << cnf[i][j] << ", ";
    }
    cout << "], ";
  }
  cout << "]" << endl;
  // print varIdTable
  for (auto it = varIdTable.cbegin(); it != varIdTable.cend(); ++it){
    cout << it->first << ": " << it->second << ", ";
  }
  cout << endl;
  return result;
}

unsigned int TseitinTransformer::getVarNum() const {
  return varIdCounter;
}

bool TseitinTransformer::checkIfLeaf(TreeNode *root){
  if (root->getContent() != "*" && root->getContent() != "+" && root->getContent() != "-"){
    return true;
  }
  else{
    return false;
  }
}

int TseitinTransformer::storeToTable(TreeNode *root){
    int curID = varIdCounter;
    auto it = varIdTable.find(root->getContent());
    if (it == varIdTable.end()){
      varIdCounter++;
      varIdTable[root->getContent()] = curID;
      //cout << "not found, add to table, return: " << curID << endl;
      return curID;
    }
    else{
      //cout << "found, taking from table, return " << it->second << endl;
      return it->second;
    }
}

int TseitinTransformer::getNumRep(){
  int largest = 0;
  for (int i=0; i!=(int)cnf.size(); i++){
    for (int j=0; j!=(int)cnf[i].size(); j++){
      if (cnf[i][j] > largest) largest = cnf[i][j];
    }
  }
  return largest;
}