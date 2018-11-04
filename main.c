#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>

#include "readin.h"
#include "check.h"
#include "opcode.h"
#include "format.h"
#include "symbol.h"
#include "process.h"

int main(int argc, char* argv[]){
	processOpCodes();
	initializeCheckTable();
	
	FILE *inputFile,*outputPrtFile, *outputExcFile;

	char* fileName = argv[1];
	currentAddress = atoi(argv[2]);

	char outputPrt[strlen(fileName)],outputExc[strlen(fileName)];

	strcpy(outputPrt, fileName);		// Format naming for both files
	strcpy(outputExc, fileName);

	strtok(outputPrt, ".");
	strtok(outputExc, ".");

	strcat(outputExc,".exc");
	strcat(outputPrt,".prt");

	currentAddress = (int)strtol(argv[2], NULL, 16);  // Get starting address

	inputFile = fopen(fileName, "r");
	
	if (inputFile == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	firstPass(inputFile,currentAddress);

	inputFile = fopen(fileName, "r");
	outputExcFile = fopen(outputExc, "w+");
	outputPrtFile = fopen(outputPrt, "w+");

	secondPass(inputFile, outputExcFile, outputPrtFile, currentAddress);

	fclose(inputFile);
	fclose(outputExcFile);
	fclose(outputPrtFile);

	return 0;
}
