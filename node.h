//---------------------------------------------------------------------------
//File name: node.h
//Author: Jacob Luciani
//Purpose: Header file for node struct used to build a data tree storing
//         info related with source parsing (parse tree)
//---------------------------------------------------------------------------

#ifndef NODE_HEADER
#define NODE_HEADER

#include <vector>
#include <string>

#include "token.h"

//c++ used for access to vectors
using namespace std;

typedef struct Node {
	
	//pointers to branches of the parse tree
	struct Node *branch1;
	struct Node *branch2;
	struct Node *branch3;
	struct Node *branch4;
	
	//holds the type of nonterminal associated with this node
	string type;
	
	//vector holds words/tokens that fall under this node
	vector<Token> members;
} Node;

#endif