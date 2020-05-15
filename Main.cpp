//---------------------------------------------------------------------------
//File name: Main.cpp
//Author: Jacob Luciani
//Purpose: Serves as an driver for a compiler to process an input file of source code
//         Generates parse tree, checks static semantics, and produces code in asm
//---------------------------------------------------------------------------

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "inputProcessing.h"
#include "Scanner.h"
#include "node.h"
#include "parser.h"
#include "StaticSem.h"
#include "Codegen.h"

using namespace std;

int main(int argc, char *argv[]) {
	
	//holds the "base" name related to the source of input
	char *baseName;
	baseName = getBaseName(argc, argv);
	string baseString(baseName);
	
	//File pointer to input file
	FILE *input;
	
	//get pointer to a file with input regardless of input source
	input = processArgs(baseName);
	
	//create Scanner object pointing to input file
	Scanner scan (input);
	
	//retrieves full parse tree, using scanner to feed parser tokens
	Node *tree = parser(scan);
	//printPreorder(tree, 0);
	
	//create semantic checker object with the tree created by the parser
	StaticSem semanticChecker(tree);
	//run check on the tree for static semantics errors
	int i = semanticChecker.checkTree();
	
	//Check for semantics error
	if (i != 0) {
		cout << "Error in static semantics. Terminating\n";
	} else {
	
		//Generate asm code
		
		//create output filename
		stringstream fileName;
		fileName << baseString << ".asm";
	
		string file = fileName.str();
	
		//open stream to output file
		ofstream output(file.c_str());
		
		//run recursive code generation
		recGen(tree, output);
	
		cout << file << " generated\n";
		output.close();
	}
	//clean up resources before termination
	fclose(input);
	free(baseName);
	freeTree(tree);
	
	
	return EXIT_SUCCESS;
}