//---------------------------------------------------------------------------
//File name: Codegen.h
//Author: Jacob Luciani - Thanks to professor Mark Hauschild for basic function
//						  structure
//Purpose: Header file for code generation functionality. Gives access to a
//         recursive function for processing a parse tree into asm
//---------------------------------------------------------------------------

#ifndef CODE_GENERATION_HEADER
#define CODE_GENERATION_HEADER

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "token.h"
#include "node.h"

using namespace std;

//lists types of temp names for generation
enum nameType {VAR, LABEL};

//generates a temporary name for a label or variable
static string newName(nameType what);

//recursively travel tree, storing output code in file with an ofstream object
void recGen(Node *root, ofstream& out); //

#endif