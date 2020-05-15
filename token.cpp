//---------------------------------------------------------------------------
//File name: token.cpp
//Author: Jacob Luciani
//Purpose: Gives implementation for stringifying the information of a token
//         also stores the names associated with the different token types
//---------------------------------------------------------------------------

#include "token.h"

//gives string descriptors of each kind of token, used to make descriptions more human-readable
string tokenNames[] = {"End of File", "Identifier", "Keyword", "Special Character", "Number" };

//returns a string description of a token's properties for output
string printToken(Token token) {
	
	stringstream buffer;

	//add all token info to buffer
	buffer << tokenNames[token.tokenId] << " " << token.tokenInstance << " at line " << token.lineNum << "\n";
	
	return buffer.str(); //convert stringstream to string
	
}