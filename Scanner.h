//---------------------------------------------------------------------------
//File name: Scanner.h
//Author: Jacob Luciani
//Purpose: Header file for Scanner class which tokenizes source code found through
//          file pointer given to the class object
//---------------------------------------------------------------------------

#ifndef SCANNER_HEADER
#define SCANNER_HEADER

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>

#include "token.h"

using namespace std;

class Scanner {
	
	private:
	
		//keeps track of what line in the source file is being processed
		int lineNum;
	
		FILE *inputFile;
	
		//file pointer "looks ahead" into source file without consuming it
		FILE *lookahead;
	
		//the next character to be processed in the file
		char lookaheadChar;
	
		//detects if EOF has been reached during processing
		bool completed;
	
		
		//used to filter out whitespace and comments while preserving line count
		//called every time the next token is requested, as such is purely internal
		void filter();
	
		//builds resource tables - FSA table and keyword lookup table
		void setupResources();
	
		//determines what kind of character is being processed, used to find column on FSA table
		static int getType(char c);
	
		//checks if an identifier token matches any of the language's keywords
		static bool isKeyword(string id);
	
	public:
		
		//constructor takes in pointer to the file to be processed
		Scanner(FILE *inputFile);
		
		//getter used to check class setup
		int getLineNum() { return lineNum; }
	
		//main public action, processes and returns the next token in the file
		Token getNextToken();
	
};

#endif