typedef int BTData;

typedef struct _binary_tree_node {
    BTData data;                       //data
    struct _binary_tree_node * left;   //left child
    struct _binary_tree_node * right;  //right child
} BinaryTreeNode;

typedef struct BinaryTree {
    struct BinaryTreeNode* root;
    int size;

} BinaryTree;

// 순회 함수에 쓰일 함수 포인터, 
// void func(BTData data) 의 함수 원형을 가리킨다.
typedef void TraversalFuctionPtr(BTData data);

BinaryTreeNode * MakeBinaryTreeNode();
BTData GetData(BinaryTreeNode * node);
void SetData(BinaryTreeNode * node, BTData data);
BinaryTreeNode * GetLeftSubTree(BinaryTreeNode * node);
BinaryTreeNode * GetRightSubTree(BinaryTreeNode * node);
void MakeLeftSubTree(BinaryTreeNode * parent, BinaryTreeNode * child);
void MakeRightSubTree(BinaryTreeNode * parent, BinaryTreeNode * child);
void RemoveTreeNode(BinaryTreeNode * root);
void PreOrderTraverse(BinaryTreeNode * root, TraversalFuctionPtr action);
void InOrderTraverse(BinaryTreeNode * root, TraversalFuctionPtr action);
void PostOrderTraverse(BinaryTreeNode * root, TraversalFuctionPtr action);

#ifndef _COMMON_LIST_DEF_
#define _COMMON_LIST_DEF_

#define TRUE		1
#define FALSE		0

#endif