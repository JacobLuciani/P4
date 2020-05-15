//---------------------------------------------------------------------------
//File name: Scanner.cpp
//Author: Jacob Luciani
//Purpose: Takes in a file of source code and gives utility to process it into
//         tokens for display/parsing
//---------------------------------------------------------------------------

#include "Scanner.h"

#include <iostream>

int fsaTable[128];
string keywords[13];

//constructor takes in pointer to the file to be processed
Scanner::Scanner(FILE *input) {
	
	//setup all pointers and set the lookahead
	this->inputFile = input;
	this->lookahead = input;
	this->lookaheadChar = fgetc(this->lookahead);
	
	this->lineNum = 1;
	
	this->completed = false;
	
	setupResources();
}

/*
setupResources builds resource tables - FSA table and keyword lookup table

FSA table is laid out as shown below: (x is an accepted state)

		        lowerCase | upperCase | digit | : | = | specialChar | WS | EOF | Else
0 Blank              5           5        4     1   1       3          -2   x     -1
1 :/=                x           x        x     x   2       x          x    x     -1
2 :=/==              x           x        x     x   x       x          x    x     -1
3 otherSpecial       x           x        x     x   x       x          x    x     -1
4 num                x           x        4     x   x       x          x    x     -1
5 identifier         5           5        5     x   x       x          x    x     -1

Implemented as a 1-d array, with row being an offset of one table width
*/
void Scanner::setupResources() {
	
	int i;
	
	//filling out FSA table
	fsaTable[0] = 5;
	fsaTable[1] = 5;
	fsaTable[2] = 4;
	fsaTable[3] = 1;
	fsaTable[4] = 1;
	fsaTable[5] = 3;
	fsaTable[6] = -2;
	fsaTable[7] = 1001;
	
	for (i = 9; i < 35; i++) {
		fsaTable[i] = 1002;
	}
	
	fsaTable[13] = 2;
		
	for (i = 36; i < 44; i++) {
		fsaTable[i] = 1003;
	}
	
	fsaTable[38] = 4;
	
	fsaTable[45] = 5;
	fsaTable[46] = 5;
	fsaTable[47] = 5;
	
	fsaTable[48] = 1004;
	fsaTable[49] = 1004;
	fsaTable[50] = 1004;
	fsaTable[51] = 1004;
	fsaTable[52] = 1004;
	
	//filling out column for characters not included in the alphabet
	for (i = 0; i < 6; i++) {
		fsaTable[i * 9 + 8] = -1;
	}
	
	//keywords list
	keywords[0] = "label";
	keywords[1] = "goto";
	keywords[2] = "loop";
	keywords[3] = "void";
	keywords[4] = "declare";
	keywords[5] = "return";
	keywords[6] = "in";
	keywords[7] = "out";
	keywords[8] = "program";
	keywords[9] = "iffy";
	keywords[10] = "then";
	keywords[11] = "assign";
	keywords[12] = "data";
	
}

//main public action, processes and returns the next token in the file
//if the file is fully processed, returns an EOF token repeatedly
Token Scanner::getNextToken() {
	
	//stores the type of token found
	TokenId nextTokenId;
	
	//if the EOF has already been reached, an EOF token will be returned
	if (completed) {
		Token eofToken = {eofTok, "EOF", lineNum};
		return eofToken;
	}
	
	//keep track of the current and next states for the state transition table
	int state;
	int nextState;
	//stores the type of the next character to be processed so that the proper column may be accessed
	int lookaheadCharType;
	
	//clear whitespace ahead of the next token if any exists
	this->filter();
	
	//start in a blank state with an empty buffer to hold processed characters
	state = 0;
	string buffer = "";
	
	//loop through processing characters until an accepted state is reached or an error is found
	while (state >= 0 && state < 1000) {
		lookaheadCharType = Scanner::getType(lookaheadChar);
		
		nextState = fsaTable[state * 9 + lookaheadCharType];
		
		if (nextState < 0) { //state < 0 indicates error
			if (nextState == -1) {
				printf("SCANNER ERROR: character on line %d not in alphabet\n", lineNum);
			} else if (nextState == -2) {
				printf("SCANNER ERROR: filter error on line %d, whitespace found in token\n", lineNum);
			}
			exit(EXIT_FAILURE);
		}
		if (nextState > 1000) { //state > 1000 indicates accepted state
			
			if (nextState == 1004) { //indicates an identifier token
				if (Scanner::isKeyword(buffer)) { //check if identifier is a keyword
					nextTokenId = kwTok;
				} else { //if not, stored as simple identifier
					nextTokenId = idTok;
				}
			} else if (nextState == 1001) { //indicates EOF token
				nextTokenId = eofTok;
				buffer.append("EOF");
				completed = true; //if EOF found, scanner has finished processing the file, so completed flag is set to true
			} else if (nextState == 1002) { //indicates a special character token
				nextTokenId = spcTok;
			} else { //otherwise, number token
				nextTokenId = numTok;
			}
			
			break;
		}
		
		//move to the next state pointed to by lookahead
		state = nextState;
		
		//add the processed character to a buffer that accumulates instance info
		buffer.push_back(lookaheadChar);
		
		//progress the lookahead pointer
		lookaheadChar = fgetc(lookahead);
	}
	
	//build token out of info found and return
	Token nextToken = { nextTokenId, buffer, lineNum};
					   
	return nextToken;
	
}

//used to filter out whitespace and comments while preserving line count
//called every time the next token is requested, as such is purely internal
void Scanner::filter() {
	
	//variable acts as flag, set to 1 when filtering is done
	int filtered;
	filtered = 0;
	
	//loop through characters until non-comment/whitespace char found
	while (filtered == 0) {
		//cout << lookaheadChar << "\n"; //debugging statement
		switch (lookaheadChar) {
			
			//check for spaces, tabs, and newlines
			case ' ':
				lookaheadChar = fgetc(lookahead);
				break;
				
			case '\t':
				lookaheadChar = fgetc(lookahead);
				break;
				
			case '\n':
				lineNum++; //record new line to keep proper track of line number
				lookaheadChar = fgetc(lookahead);
				break;
				
			case '#':
				//for comment, loop through (still recording line changes) until a closing pound symbol is found
				do {
					lookaheadChar = fgetc(lookahead);
					if (lookaheadChar == '\n') {
						lineNum++;
					}
				} while (lookaheadChar != '#' && lookaheadChar != EOF);
				
				lookaheadChar = fgetc(lookahead); //increment past the closing tag
				break;
				
			default: //if none of the above are found, the scanner has filtered up to the next token
				filtered = 1;
				break;
		}
	}
}

//determines what kind of character is being processed, number is used to find column on FSA table
int Scanner::getType(char c) {
	
	if (isalpha(c) != 0) { //check for alpha and distinguish upper/lowercase
		
		if (isupper(c) == 0) {
			return 0;
		} else if (isupper(c) != 0) {
			return 1;
		}
		
	} else if (isdigit(c)) { //check for number
		return 2;
	} else if ( c == ':' ) { //check for colon - to be used to find ':=' token as well as ':' token
		return 3;
	} else if ( c == '=') { //check for equals sign = to be used to find '=', ':=', and '==' tokens
		return 4;
	} else if ( c == '<' || c == '>' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '.' || c == '(' || c == ')' || c == ',' || c == '{' || c == '}' || c == ';' || c == '[' || c == ']') {
		return 5; //catches all other special chars
	} else if ( c == '\n' || c == ' ' || c == '\t' ) { //check if whitespace
		return 6;
	} else if ( c == EOF ) { //check for end of file
		return 7;
	} else { //if none of the above, character is not in the language's alphabet
		return 8;
	}

}

//checks if an identifier token matches any of the language's keywords
bool Scanner::isKeyword(string id) {
	
	int i;
	
	//loop through keyword list and check for matches
	for (i = 0; i < 13; i++) {
		if (id.compare(keywords[i]) == 0) {
			return true;
		}
	}
	//if no matches found, the input identifier is not a keyword
	return false;
}

