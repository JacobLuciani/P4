//---------------------------------------------------------------------------
//File name: token.h
//Author: Jacob Luciani
//Purpose: Defines the Token struct to hold info about a single token produced by lexical analysis
//         Also provides an enum for token types and a header for print functionality
//---------------------------------------------------------------------------

#ifndef TOKEN_HEADER
#define TOKEN_HEADER

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

//types of token allowed
enum TokenId { eofTok, idTok, kwTok, spcTok, numTok };

//holds info describing a token identified during lexical analysis
typedef struct Token {
	
	TokenId tokenId;         //type of token
	string tokenInstance;    //details of specific contents of token
	int lineNum;             //line in source code where token was found
	
} Token;

//returns a string description of a token's properties for output
string printToken(Token token);

#endif