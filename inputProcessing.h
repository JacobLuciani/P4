//---------------------------------------------------------------------------
//File name: inputProcessing.h
//Author: Jacob Luciani
//Purpose: Gives interface for input processing functions
//         Functions give common access point for input regardless of source
//---------------------------------------------------------------------------

#ifndef INPUT_PROCESSING_HEADER
#define INPUT_PROCESSING_HEADER

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//Takes in command line arguments and determines source of information, to be used for processing input
char *getBaseName(int argc, char *argv[]);

//Takes in input source base name and returns working file pointer associated with data
//handles and converges different program states/types of input
FILE *processArgs(char *baseName);

//prompts user for string input to be put in temp file for processing
void getInput(FILE *input);

#endif