//---------------------------------------------------------------------------
//File name: StaticSem.cpp
//Author: Jacob Luciani
//Purpose: Takes in a pointer to a parse tree and checks it for proper static
//         semantics. Creates a list of defined variables, reports error for duplicates
//		   and use without definition
//---------------------------------------------------------------------------

#include <sstream>

#include "StaticSem.h"

using namespace std;

//constructro takes in a parse tree and prepares its data structure to store variable names
StaticSem::StaticSem(Node *root) {
	
	this->variables.empty();
	
	this->root = root;
}


//adds identifier to list of variables on definition, checking for erroneous redefintion
void StaticSem::insert(Node *root) {
	
	//declaration nodes store nonterminals as: identifier number ;
	//thus the identifier will always be the first element, and must exist or it would not have passed the parser
	Token identifier = root->members[0];
	
	//search for identifier in list of variables to ensure no double definition 
	if (find(variables.begin(), variables.end(), identifier.tokenInstance) == variables.end()) {
		//if no match is found, add variable  to current the list
		variables.push_back(identifier.tokenInstance);
		//cout << "Added " << variables.back() << " to list of variables\n";
	} else {
		cout << "Error found in variable definition: duplicate definition\n";
		exit(1);
	}
}

//checks list for identifier when found, ensuring it has been previously defined
int StaticSem::verify(Token identifier) {
	//cout << "Variable " << identifier.tokenInstance << " used on line " << identifier.lineNum << "\n";
	
	//check if identifier found corresponds to variable that has been previously defined
	if (find(variables.begin(), variables.end(), identifier.tokenInstance) == variables.end()) {
		//if no match is found, return error indicator
		return 1;
	}
	//if a match is found, return success indicator
	return 0;
}

//traverses parse tree in preorder to check for proper static semantics
//Uses current object's tree's root to start recursive call (recursive helper function)
//allows public access to functionality while restricting the application to only the present object
int StaticSem::checkTree() {
	checkBranch(this->root);
	//cout << "Success: parse tree adheres to static semantic rules\n";
	return 0;
}

//recursively checks branches of tree
void StaticSem::checkBranch(Node *root) {
	
	if (root != NULL) {
		
		//check if variable is being defined, indicated by production type "vars"
		if (root->type == "vars" ) {
			//cout << "Vars found\n";
			
			//add the defined variable to the StaticSem object's list of defined variables
			insert(root);
		} else { 
			//check if an identifer is used
			//loop through the terminals of the current node, checking for idToks
			for (int i = 0; i < root->members.size(); i++ ) {
				if ( root->members[i].tokenId == idTok ) {
					
					//identifier found, check for prior definition (inclusion in the variables list)
					int result = verify(root->members[i]);
					
					//if nonzero return, error encountered
					if (result == 1) {
						cout << "Error: variable " << root->members[i].tokenInstance << " used on line " << root->members[i].lineNum << " without prior definition\n";
						exit(1);
					}
				}
			}
		}
		
		//check all branches of the current node
		checkBranch(root->branch1);
		checkBranch(root->branch2);
		checkBranch(root->branch3);
		checkBranch(root->branch4);
	}
}