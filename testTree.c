//---------------------------------------------------------------------------
//File name: testTree.c
//Author: Jacob Luciani
//Purpose: Contains operations for a parse tree's construction,
//         preorder traversal, and destruction.
//---------------------------------------------------------------------------

#include "testTree.h"

//Creates and returns pointer to Node object with fields cleared properly
Node *buildNode(string type) {
	
	//allocate space for root of tree
	Node *tree = new Node;
	
	//ensures all fields are empty post-creation
	tree->members.empty();
	tree->branch1 = NULL;
	tree->branch2 = NULL;
	tree->branch3 = NULL;
	tree->branch4 = NULL;
	
	//cout << "type is " << type <<"\n";
	
	tree->type = type;
	
	//cout << tree->type << " type assigned\n";
	
	return tree;
}

//adds token to a specific node
void addToNode(Node *tree, Token token) {
	tree->members.push_back(token);
}

//Prints formatted info about one node in the tree with indentation according to "level"
void printNode(Node *tree, int level) {
	
	//ensure first level is properly non-indented
	if (level == 0) {
		cout << "Level " << level << ": Type: " << tree->type << " Terminals:";
	} else {
		//print out node info, indented two spaces per level on the tree
		cout << string(level * 2, ' ') << "Level " << level << ": Type: " << tree->type << ": Terminals:";
	}
	
	//print out all members of the node on the same line
	for (int i = 0; i < tree->members.size(); i++) {
		cout << " " << tree->members[i].tokenInstance;
	}
	cout << "\n";
	
}


//Print function prints tree in preorder
void printPreorder(Node *tree, int level) {
	//if the node exists, recursively print the node in preorder (root-branch1-branch2-branch3-branch4)
	if (tree != NULL) {
		printNode(tree, level);
		printPreorder(tree->branch1, level + 1);
		printPreorder(tree->branch2, level + 1);
		printPreorder(tree->branch3, level + 1);
		printPreorder(tree->branch4, level + 1);
	}
}


//Traverses tree and releases memory for nodes of tree
void freeTree(Node *tree) {
	if (tree != NULL) {
		freeTree(tree->branch1);
		freeTree(tree->branch2);
		freeTree(tree->branch3);
		freeTree(tree->branch4);
		delete tree;
	}
}