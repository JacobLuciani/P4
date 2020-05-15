//---------------------------------------------------------------------------
//File name: testTree.h
//Author: Jacob Luciani
//Purpose: Header file for a parse tree, function declarations for
//         construction, preorder traversal, and destruction.
//---------------------------------------------------------------------------

#ifndef TREE_HEADER
#define TREE_HEADER

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <algorithm>

#include "node.h"

//Creates and returns pointer to Node object with fields cleared properly
Node *buildNode(string type);

//adds token to a specific node
void addToNode(Node *tree, Token token);

//Prints formatted info about one node in the tree with indentation according to "level"
void printNode(Node *tree, int level);

//Print function prints tree in preorder
void printPreorder(Node *tree, int level);

//Traverses tree and releases memory for nodes of tree
void freeTree(Node *tree);

#endif