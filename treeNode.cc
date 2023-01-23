#include "treeNode.h"
#include "exceptionHandler.h"
#include <sstream>
#include <iostream>
using namespace std;

TreeNode::TreeNode(string type, string cntt): type(type), content(cntt) {}
//TreeNode::TreeNode(){}

string TreeNode::getContent() const {
  return content;
}

string TreeNode::getType() const {
  return type;
}

TreeNode *TreeNode::getLeftChild() const {
  return leftChild;
}
TreeNode *TreeNode::getRightChild() const {
  return rightChild;
}

void TreeNode::updateLeftChild(TreeNode *lChild) {
  leftChild = lChild;
}

void TreeNode::updateChildren(TreeNode *lChild, TreeNode *rChild) {
  leftChild = lChild;
  rightChild = rChild;
}

TreeNode::~TreeNode() {
  // your code starts here
}

//AllTypeNode::AllTypeNode(string type, string cntt):type{type},cntt{cntt}{}

bool EvaluateTree::postOrderTraversal(TreeNode* node) {
  bool leftContent;
  bool rightContent;
  bool hasLeft = false;
  bool hasRight = false;
  //cout << "node type: " << node->getType() << endl;
  //cout << "node content: " << node->getContent() << endl;
  if (node->getLeftChild() != nullptr){
    leftContent = postOrderTraversal(node->getLeftChild());
    hasLeft = true;
  }
  if (node->getRightChild() != nullptr){
    rightContent = postOrderTraversal(node->getRightChild());
    hasRight = true;
  }
  // parent
  /*cout << "(parent): " << endl;
  cout << "type: " << node->getType() << endl;
  cout << "content: " << node->getContent() << endl;
  cout << "leftContent: " << leftContent << endl;
  cout << "rightContent: " << rightContent << endl;*/
  if (node->getType() == "constant"){
    bool b;
    istringstream(node->getContent()) >> b;
    //cout << b << endl;
    return b;
  }
  else if (node->getType() == "variable"){
    /*map<string, bool>::iterator itt;
    for (itt=assgm.begin();itt!=assgm.end();++itt){
      cout << itt->first << "<-" << itt->second << endl;
    }*/ 
    //if (assgm.size()==0){ exceptionFlag = true; }
    //cout << node->getContent() << endl;
    map<string, bool>::const_iterator it = assgm.find(node->getContent());
    // if not finding the key
    if (it == assgm.end()){
      //throw parserException("(threw) (no assignment) Error: incomplete assignment");
      throw parserException("Error: incomplete assignment");
    } else {
      return it->second;
    }
    //auto it = assgm.find(node->getContent());

  }
  else{
    if (node->getContent() == "+" && hasLeft && hasRight){
      //cout << "+" << endl;
      return leftContent || rightContent;
    }
    else if (node->getContent() == "*" && hasLeft && hasRight){
      //cout << "*" << endl;
      return leftContent && rightContent;
    }
    else if (node->getContent() == "-" && hasLeft && !hasRight){
      //cout << "-" << endl;
      return !leftContent;
    }
    else{ 
      //throw parserException("(unexpected operator) Error: invalid input");
      throw parserException("Error: invalid input");  
    }
  }
  assgm.clear();
}

/* 
OperatorNode::OperatorNode(string cntt): TreeNode{cntt} {}

bool OperatorNode::evaluate(const map<string, bool> &assignment) const {
  // your code starts here
  return false;
}

ConstantNode::ConstantNode(string cntt): TreeNode{cntt} {}

bool ConstantNode::evaluate(const map<string, bool> &assignment) const {
  // your code starts here
  return false;
}

VariableNode::VariableNode(string cntt): TreeNode{cntt} {}

bool VariableNode::evaluate(const map<string, bool> &assignment) const {
  // your code starts here
  return false;
}*/

