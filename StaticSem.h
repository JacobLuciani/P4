//---------------------------------------------------------------------------
//File name: StaticSem.h
//Author: Jacob Luciani
//Purpose: Provides class definition for a static semantic checker that takes
//         a parse tree and is able to traverse the tree checking for errors
//---------------------------------------------------------------------------

#ifndef STATIC_SEMANTICS_HEADER
#define STATIC_SEMANTICS_HEADER

#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "token.h"
#include "node.h"

using namespace std;

class StaticSem {
	
	private:
	
		//holds all currently declared variables
		vector<string> variables;
	
		//holds reference to the root of the parse tree
		Node *root;
		
		//adds identifier to list of variables on definition, checking for erroneous redefintion
		void insert(Node *root);

		//checks list for identifier when found, ensuring it has been previously defined
		int verify(Token identifier);
	
		//recursively checks branches of tree
		void checkBranch(Node *root);
	
	public:
		
		//constructor takes in pointer to the file to be processed
		StaticSem(Node *root);
		
		//traverses parse tree in preorder to check for proper static semantics
		//starts recursive traversal at the object's tree's root node
		int checkTree();
};


#endif