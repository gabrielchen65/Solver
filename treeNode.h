#ifndef TNODE_H
#define TNODE_H

#include <string>
#include <map>
using namespace std;

// abstract superclass of the syntax tree node
class TreeNode {
    string type;
    string content;
    TreeNode *leftChild = nullptr;
    TreeNode *rightChild = nullptr;
  public:
    TreeNode *getLeftChild() const;
    TreeNode *getRightChild() const;
    TreeNode(string type, string cntt);
    string getContent() const;
    string getType() const;
    //virtual bool evaluate(const map<string, bool> &assignment) const = 0; // pure virtual method
    void updateLeftChild(TreeNode *lChild); // for the operator "-", the operand is stored in leftChild
    void updateChildren(TreeNode *lChild, TreeNode *rChild);
    virtual ~TreeNode();
};

// concrete tree node of all type
/*
class AllTypeNode : public TreeNode {
  public:
    AllTypeNode(string type, string cntt);
};*/

class EvaluateTree{
    map<string, bool> assgm;
  public:
    EvaluateTree(map<string, bool> assgm):assgm(assgm){};
    bool postOrderTraversal(TreeNode* node);
};

/*
// concrete tree node of opertaor type
class OperatorNode : public TreeNode {
  public:
    OperatorNode(string cntt);
    bool evaluate(const map<string, bool> &assignment) const override;
};

// concrete tree node of constant type
class ConstantNode : public TreeNode {
  public:
    ConstantNode(string cntt);
    bool evaluate(const map<string, bool> &assignment) const override;
};

// concrete tree node of varibale type
class VariableNode : public TreeNode {
  public:
    VariableNode(string cntt);
    bool evaluate(const map<string, bool> &assignment) const override;
};*/


#endif
