//---------------------------------------------------------------------------
//File name: parser.h
//Author: Jacob Luciani
//Purpose: Provides interface with parse tree-generating functionality
//         accepts a Scanner object as a token source and builds a parse tree
//		   according to predefined grammar, returns tree as a Node-based data
//         structure
//---------------------------------------------------------------------------

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include "token.h"
#include "inputProcessing.h"
#include "Scanner.h"
#include "node.h"
#include "testTree.h"

using namespace std;

//Main point of contact- takes in Scanner object, initiates parse tree creation, and returns finished tree
//if no errors are found
Node *parser(Scanner scan);

//Each of the following represents the code form of the production rule associated with the nonterminal for which
//the function is named, with the exception of ifNonTerminal and gotoNonterminal, which represent if and goto respectively
//due to their status as keywords
Node *program();
Node *block();
Node *vars();
Node *expr();
Node *N();
Node *A();
Node *M();
Node *R();
Node *stats();
Node *mStat();
Node *stat();
Node *in();
Node *out();
Node *ifNonTerminal();
Node *loop();
Node *assign();
Node *label();
Node *gotoNonTerminal();
Node *RO();
