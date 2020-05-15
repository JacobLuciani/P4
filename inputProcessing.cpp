//---------------------------------------------------------------------------
//File name: inputProcessing.cpp
//Author: Jacob Luciani
//Purpose: Takes in and handles program arguments to process input information
//         Provides a single way of accessing multiple different kinds of input source
//---------------------------------------------------------------------------

#include "inputProcessing.h"

//Takes in command line arguments and determines the base name to be used for the input file pointer based on the data source
char *getBaseName(int argc, char *argv[]) {
	
	char *fileName;
	
	if (argc > 1) { //file passed as argument
		
		//allocated space for filename to be stored
		fileName = (char *) malloc(sizeof(argv[1]));
		
		sprintf(fileName, "%s%c", argv[1], '\0');
		
	} else { //no file passed, keyboard/piped input
		fileName = (char *) malloc(7);
		
		sprintf(fileName, "kb%c", '\0');
	}
	
	return fileName;
}

//Takes in input source base name and returns working file pointer associated with data
//handles and converges different program states/types of input
FILE *processArgs(char *baseName) {
	
	char *inputFileName;
	FILE *input;
	
	//if the base name is kb, data is being passed via keyboard
	//otherwise, it is a file
	if (strcmp(baseName, "kb") != 0) { //file passed as argument
		
		//allocated space for filename to be stored
		inputFileName = (char *) malloc(sizeof(baseName) + 8);
		
		//add implicit extension
		sprintf(inputFileName, "%s.sp2020", baseName);

		//open file pointer to given filename
		input = fopen(inputFileName, "r");
		
		//check if file could be opened, if not print error message and terminate
		if (input == NULL) {
			printf("Input file name %s could not be opened. Terminating.\n", inputFileName);
			
			free(inputFileName);
			exit(1);
		}
				
		//free variable storing file name with extension
		free(inputFileName);	
	} else {
		//check if data has been piped in from a file
		//if not, open file and request input from user to be put in file
		//otherwise, set input to stdin and move on
		if (isatty(STDIN_FILENO)) { //input to be typed in by keyboard
			input = fopen("tempInput.sp2020", "w+");
			getInput(input);
		} else { //input piped in
			input = stdin;
		}
	}
	
	//check if input file has any content, error if none
	char word[128];
	
	if (fscanf(input, "%s", word) == EOF) {
		printf("Empty input file. Terminating.\n");
		exit(1);
	}
	
	//printf("file found\n");
	//return to start of file after checking for content and return file pointer
	fseek(input, 0, SEEK_SET);
	return input;
}

//prompts user for input and stores it in file given by input pointer
void getInput(FILE *input) {
	
	printf("Input needed. Please enter input strings, enter '!' to end input: \n");
	
	//variable holds each "word" of input individually
	char word[128];
	
	//clears out word to allow do-while loop to print before reading the next word
	sprintf(word, "%c", '\0');
	
	//prints then reads next word of input from user
	//allows the loop to stop before writing the simulated EOF to the file pointer
	do {
		fprintf(input, "%s ", word);
		scanf("%s", word);
	} while (strcmp(word, "!") != 0);
		
	//return file pointer to start of file
	fseek(input, 0, SEEK_SET);
}