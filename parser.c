//---------------------------------------------------------------------------
//File name: parser.c
//Author: Jacob Luciani
//Purpose: Takes in token stream from scanner and builds parse tree based on
//         a predefined BNF grammar. Parse tree is built as a node tree
//---------------------------------------------------------------------------

#include "parser.h"

//variables must be accessible to all functions for ease of use/communication
Scanner *scan;
Token token;

//Main point of contact- takes in Scanner object, initiates parse tree creation, and returns finished tree
//if no errors are found
Node *parser(Scanner inScan) {
	
	scan = &inScan;
	token = scan->getNextToken();
	
	//debug line
	//cout << printToken(token) << "Made it into parser\n";
	
	Node *root = program();
	
	
	if ( token.tokenId == eofTok ) {
		//cout << "File successfully parsed in full\n";
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": fully resolved parse tree did not consume all tokens\n";
		exit(1);
	}
	
	return root;
}

//Represents the production rule associated with the <program> nonterminal
Node *program() {
	//cout << printToken(token) << "Made it into parser\n";
	
	Node *program = buildNode("program");
	
	program->branch1  = vars();
	program->branch2 = block();
	
	return program;
}

//Represents the production rule associated with the <block> nonterminal
Node *block() {
	//cout << "Block starting at line " << scan->getLineNum() << "\n";
	
	Node *blockNode = buildNode("block");

	//check for structure { <vars> <stats> }
	//add terminals to current node if they are found
	if ( token.tokenId == spcTok && token.tokenInstance == "{" ) {
		token = scan->getNextToken();
		
		blockNode->branch1 = vars();
		blockNode->branch2 = stats();
		
		if ( token.tokenId == spcTok && token.tokenInstance == "}" ) {
			token = scan->getNextToken();
			
			return blockNode;
		} else { //} not found
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected } token at end of block\n";
			exit(1);
		}
	} else { //{ not found
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected { token at start of block\n";
		exit(1);
	}
}

//Represents the production rule associated with the <vars> nonterminal
Node *vars() {
	//cout << "Vars starting at line " << scan->getLineNum() << "\n";
	
	Node *varsNode = buildNode("vars");
	
	//check for structure declare Identifier := Number ; <vars>
	//add terminals to current node if they are found
	if ( token.tokenId == kwTok && token.tokenInstance == "declare" ) {
		token = scan->getNextToken();
		
		if ( token.tokenId == idTok ) {
			addToNode(varsNode, token);
			token = scan->getNextToken();
			
			if ( token.tokenId == spcTok && token.tokenInstance == ":=" ) {
				token = scan->getNextToken();
				
				if ( token.tokenId == numTok ) {
					addToNode(varsNode, token);
					token = scan->getNextToken();
					
					if ( token.tokenId == spcTok && token.tokenInstance == ";" ) {
						token = scan->getNextToken();
						
						varsNode->branch1 = vars();
						return varsNode;
					} else { //; not found
						cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected ; token\n";
						exit(1);
					}
				} else { //number not found
					cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected number token on rhs of variable declaration\n";
					exit(1);
				}
			} else { //:= not found
				cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected := token\n";
				exit(1);
			}
		} else { //identifier not found
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected identifier on lhs of variable declaration\n";
			exit(1);
		}
	} else { //declare not found
		freeTree(varsNode);
		return NULL;
	}
}

//Represents the production rule associated with the <expr> nonterminal
Node *expr() {
	//cout << "Expr starting at line " << scan->getLineNum() << "\n";
	
	Node *exprNode = buildNode("expr");
	
	exprNode->branch1 = N();
	
	//check for - <expr>
	//add terminals to current node if they are found
	if ( token.tokenId == spcTok && token.tokenInstance == "-" ) {
		addToNode(exprNode, token);
		token = scan->getNextToken();
		
		exprNode->branch2 = expr();
		return exprNode;
	} else {
		return exprNode;
	}
}

//Represents the production rule associated with the <N> nonterminal
Node *N() {
	//cout << "N starting at line " << scan->getLineNum() << "\n";
	
	Node *nNode = buildNode("N");
	
	nNode->branch1 = A();
	
	//check for / <expr> or * <expr>
	//add terminals to current node if they are found
	if ( token.tokenId == spcTok && (token.tokenInstance == "/" || token.tokenInstance == "*") ) {
		addToNode(nNode, token);
		token = scan->getNextToken();
		
		nNode->branch2 = N();
		return nNode;
	} else {
		return nNode;
	}
}

//Represents the production rule associated with the <A> nonterminal
Node *A() {
	//cout << "A starting at line " << scan->getLineNum() << "\n";
	
	Node *aNode = buildNode("A");
	
	aNode->branch1 = M();
	
	//check for + <A>
	//add terminals to current node if they are found
	if (token.tokenId == spcTok && token.tokenInstance == "+" ) {
		addToNode(aNode, token);
		token = scan->getNextToken();
		
		aNode->branch2 = A();
		return aNode;
	} else {
		return aNode;
	}
}

//Represents the production rule associated with the <M> nonterminal
Node *M() {
	//cout << "M starting at line " << scan->getLineNum() << "\n";
	
	Node *mNode = buildNode("M");
	
	//check for * <M>
	//add terminals to current node if they are found
	if ( token.tokenId == spcTok && token.tokenInstance == "*" ) {
		addToNode(mNode, token);
		token = scan->getNextToken();
		
		mNode->branch1 = M();
		return mNode;
	} else {
		mNode->branch1 = R();
		return mNode;
	}
}

//Represents the production rule associated with the <R> nonterminal
Node *R() {
	//cout << "R starting at line " << scan->getLineNum() << "\n";
	
	Node *rNode = buildNode("R");
	
	//check for expected structure
	//add terminals to current node if they are found
	if ( token.tokenId == spcTok && token.tokenInstance == "(" ) { // ( <expr> )
		token = scan->getNextToken();
		
		rNode->branch1 = expr();
		
		if ( token.tokenId == spcTok && token.tokenInstance == ")" ) {
			token = scan->getNextToken();
			
			return rNode;
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected ) token\n";
			exit(1);
		}
	} else if ( token.tokenId == idTok || token.tokenId == numTok ) { // number or identiefier found
		addToNode(rNode, token);
		//cout << "Id or num found: " << token.tokenInstance  << "\n";
		token = scan->getNextToken();
		return rNode;
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected ( token or number/identifier\n";
		exit(1);
	}
}

//Represents the production rule associated with the <stats> nonterminal
Node *stats() {
	Node *statsNode = buildNode("stats");
	statsNode->branch1 = stat();
	statsNode->branch2 = mStat();
	return statsNode;
}

//Represents the production rule associated with the <mStat> nonterminal
Node *mStat() {
	//cout << "mStat starting at line " << scan->getLineNum() << "\n";
	
	Node *mStatNode = buildNode("mStat");
	
	//checks for elements of the First() set of <stat> to see if another <stat> is needed
	if ( token.tokenId == kwTok || (token.tokenId == spcTok && token.tokenInstance == "{") || token.tokenId == idTok ) {
		mStatNode->branch1 = stat();
		mStatNode->branch2 = mStat();
		return mStatNode;
	} else { //no more <stat>'s needed, so empty production rule is used
		freeTree(mStatNode);
		return NULL;
	}
}

//Represents the production rule associated with the <stat> nonterminal
Node *stat() {
	//cout << "Stat starting at line " << scan->getLineNum() << "\n";
	
	Node *statNode = buildNode("stat");
	
	//uses next token to find which production rule needs to be used, pipes to appropriate rule if found
	if ( token.tokenId == spcTok && token.tokenInstance == "{" ) {
		statNode->branch1 = block();
	} else {
		if ( token.tokenId == idTok ) {
			statNode->branch1 = assign();
		} else if ( token.tokenId == kwTok ) {
			if ( token.tokenInstance == "in" ) {
				statNode->branch1 = in();
			} else if ( token.tokenInstance == "out" ) {
				statNode->branch1 = out();
			} else if ( token.tokenInstance == "iffy" ) {
				statNode->branch1 = ifNonTerminal();
			} else if ( token.tokenInstance == "loop" ) {
				statNode->branch1 = loop();
			} else if ( token.tokenInstance == "label" ) {
				statNode->branch1 = label();
			} else if ( token.tokenInstance == "goto" ) {
				statNode->branch1 = gotoNonTerminal();
			} else {
				cout << "PARSER ERROR: Line " << scan->getLineNum() << ": invalid keyword token\n";
				exit(1);
			}
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": invalid token\n";
			exit(1);
		}
		if ( token.tokenId == spcTok && token.tokenInstance == ";" ) { //checks for ; after all options except <block>
			token = scan->getNextToken();
			
			return statNode;
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": invalid token\n";
			exit(1);
		}
	}
}

//Represents the production rule associated with the <in> nonterminal
Node *in() {
	//cout << "In starting at line " << scan->getLineNum() << "\n";
	
	Node *inNode = buildNode("in");
	
	//check for correct structure
	//add terminals to current node if they are found
	if ( token.tokenId == kwTok && token.tokenInstance == "in" ) {
		token = scan->getNextToken();
		
		if ( token.tokenId == idTok ) {
			addToNode(inNode, token);
			token = scan->getNextToken();
			return inNode;
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected identifier token\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected \"in\" token\n";
		exit(1);
	}
}

//Represents the production rule associated with the <out> nonterminal
Node *out() {
	//cout << "Out starting at line " << scan->getLineNum() << "\n";
	
	Node *outNode = buildNode("out");
	
	//check for correct structure
	//add terminals to current node if they are found
	if ( token.tokenId == kwTok && token.tokenInstance == "out" ) {
		token = scan->getNextToken();
		outNode->branch1 = expr();
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected \"out\" token\n";
		exit(1);
	}
}

//Represents the production rule associated with the <if> nonterminal
Node *ifNonTerminal() {
	//cout << "Iffy starting at line " << scan->getLineNum() << "\n";
	
	Node *ifNode = buildNode("if");
	
	//check for correct structure: <if> [ <epxr> <RO> <expr> ] then <stats>
	//add terminals to current node if they are found
	if ( token.tokenId == kwTok && token.tokenInstance == "iffy" ) {
		token = scan->getNextToken();
		
		if ( token.tokenId == spcTok && token.tokenInstance == "[" ) {
			token = scan->getNextToken();
			
			ifNode->branch1 = expr();
			ifNode->branch2 = RO();
			ifNode->branch3 = expr();
			
			if ( token.tokenId == spcTok && token.tokenInstance == "]" ) {
				token = scan->getNextToken();
				
				if ( token.tokenId == kwTok && token.tokenInstance == "then" ) {
					token = scan->getNextToken();
					
					ifNode->branch4 = stat();
					return ifNode;
				} else {
					cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected \"then\" token\n";
					exit(1);
				}
			} else {
				cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected ] token \n";
				exit(1);
			}
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected [ token\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected \"iffy\" token\n";
		exit(1);
	}
}

//Represents the production rule associated with the <loop> nonterminal
Node *loop() {
	//cout << "Loop starting at line " << scan->getLineNum() << "\n";
	
	Node *loopNode = buildNode("loop");
	
	//check for correct structure: <loop> [ <expr> <RO> <expr> ] then <stat>
	//add terminals to current node if they are found
	if ( token.tokenId == kwTok && token.tokenInstance == "loop" ) {
		token = scan->getNextToken();
		
		if ( token.tokenId == spcTok && token.tokenInstance == "[" ) {
			token = scan->getNextToken();
			
			loopNode->branch1 = expr();
			loopNode->branch2 = RO();
			loopNode->branch3 = expr();
			
			if ( token.tokenId == spcTok && token.tokenInstance == "]" ) {
				token = scan->getNextToken();
				
				loopNode -> branch4 = stat();
				return loopNode;
			} else {
				cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected ] token\n";
				exit(1);
			}
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected [ token\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected \"loop\"\n";
		exit(1);
	}
}

//Represents the production rule associated with the <assign> nonterminal
Node *assign() {
	//cout << "Assign starting at line " << scan->getLineNum() << "\n";
	
	Node *assignNode = buildNode("assign");
	
	//check for correct structure: Identifier := <expr>
	//add terminals to current node if they are found
	if ( token.tokenId == idTok ) {
		addToNode(assignNode, token);
		token = scan->getNextToken();
		
		if ( token.tokenId == spcTok && token.tokenInstance == ":=" ) {
			token = scan->getNextToken();
			
			assignNode->branch1 = expr();
			return assignNode;
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected := token\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected identifier\n";
		exit(1);
	}
}

//Represents production rule associated with <label> nonterminal
Node *label() {
	//cout << "Label starting at line " << scan->getLineNum() << "\n";
	
	Node *labelNode = buildNode("label");
	
	//check for correct structure
	//add terminals to current node if they are found
	if ( token.tokenId == kwTok && token.tokenInstance == "label" ) {
		token = scan->getNextToken();
		
		if ( token.tokenId == idTok ) {
			addToNode(labelNode, token);
			token = scan->getNextToken();
			
			return labelNode;
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected identifier\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected \"label\" token\n";
		exit(1);
	}
}

//Represents the production rule associated with the <goto> nonterminal
Node *gotoNonTerminal() {
	//cout << "Goto starting at line " << scan->getLineNum() << "\n";
	
	Node *gotoNode = buildNode("goto");
	
	//check for correct structure
	//add terminals to current node if they are found
	if ( token.tokenId == kwTok && token.tokenInstance == "goto" ) {
		token = scan->getNextToken();
		if ( token.tokenId == idTok ) {
			addToNode(gotoNode, token);
			token = scan->getNextToken();
			return gotoNode;
		} else {
			cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected identifier\n";
			exit(1);
		}
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected \"goto\" token\n";
		exit(1);
	}
}

//Represents the production rule associated with the <RO> nonterminal
Node *RO() {
	//cout << "RO starting at line " << scan->getLineNum() << "\n";
	
	Node *rONode = buildNode("RO");
	
	//check for </<</<>/>>/>/==
	//add terminals to current node if they are found
	if ( token.tokenId == spcTok && token.tokenInstance == "<" ) {
		addToNode(rONode, token);
		token = scan->getNextToken();
		
		//check for extra < to make <<
		if ( token.tokenId == spcTok && token.tokenInstance == "<" ) {
			addToNode(rONode, token);
			token = scan->getNextToken();
			
			return rONode;
		} else if ( token.tokenId == spcTok && token.tokenInstance == ">" ) { //check for > to make <>
			addToNode(rONode, token);
			token = scan->getNextToken();
			
			return rONode;
		} else {
			return rONode;
		}
	} else if ( token.tokenId == spcTok && token.tokenInstance == ">" ) {
		addToNode(rONode, token);
		token = scan->getNextToken();
		
		//check for extra > to make >>
		if ( token.tokenId == spcTok && token.tokenInstance == ">" ) {
			addToNode(rONode, token);
			token = scan->getNextToken();
			
			return rONode;
		} else {
			return rONode;
		}
	} if ( token.tokenId == spcTok && token.tokenInstance == "==" ) {
		addToNode(rONode, token);
		token = scan->getNextToken();
		
		return rONode;
	} else {
		cout << "PARSER ERROR: Line " << scan->getLineNum() << ": expected comparison special character\n";
		exit(1);
	}
}
