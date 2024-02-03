#include "BinaryTree.h"
#include <stdio.h>


BinaryTreeNode * MakeBinaryTreeNode(){

    BinaryTreeNode *tree;

    tree = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));

    if(tree != NULL)
    {
        tree->left = NULL;
        tree->right = NULL;
        tree->data = 0;
        return (tree);
    }
    else{
        return FALSE;
    }

};

BTData GetData(BinaryTreeNode * node){
    return node->data;

};

void SetData(BinaryTreeNode * node, BTData data){
    node->data = data;
};

BinaryTreeNode * GetLeftSubTree(BinaryTreeNode * node){
    return node->left;
};

BinaryTreeNode * GetRightSubTree(BinaryTreeNode * node){
    return node->right;
};

void MakeLeftSubTree(BinaryTreeNode * parent, BinaryTreeNode * child){

    if (parent-> left != NULL){
        RemoveTreeNode(parent->left);
    }

    parent->left = child;

};

void MakeRightSubTree(BinaryTreeNode * parent, BinaryTreeNode * child){

    if (parent->right != NULL){
        RemoveTreeNode(parent->right);
    }

    parent->right = child;
};

void RemoveTreeNode(BinaryTreeNode * root){

    if(root == NULL){
        return;
    }

    RemoveTreeNode(root->left);
    RemoveTreeNode(root->right);
    free(root);
    root = NULL;

};

void PreOrderTraverse(BinaryTreeNode * root, TraversalFuctionPtr action){

    if (root == NULL)
        return ;

    action(root->data);
    PreOrderTraverse(root->right,action);
    PreOrderTraverse(root->left,action);

};

void InOrderTraverse(BinaryTreeNode * root, TraversalFuctionPtr action){

    if (root == NULL)
        return ;

    InOrderTraverse(root->right,action);
    action(root->data);
    InOrderTraverse(root->left, action);

};

void PostOrderTraverse(BinaryTreeNode * root, TraversalFuctionPtr action){

    if (root == NULL)
        return ;
        
    PostOrderTraverse(root->right,action);
    PostOrderTraverse(root->left, action);
    action(root->data);

};