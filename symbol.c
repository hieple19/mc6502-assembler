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


// Symbol struct has a symbol name and an address
Symbol* symbolTable[500];
int symbolCount = 0;

bool checkDuplicate(char* name){
	for(int i = 0; i<symbolCount; i++){
		if(!strcmp(symbolTable[i]->name,name)){
			return false;
		}
	}
	return true;
}
// Read and store new symbols in array
void updateSymbol(char* name, int address){
	if(checkDuplicate(name)){
		symbolTable[symbolCount] = (Symbol*)malloc(sizeof(Symbol));
		Symbol* current = symbolTable[symbolCount];
		current->name = name;
		current->address = address;
		symbolCount++;
	}
}

// Print out all symbols in the symbol table
void printSymbols(FILE* outputExcFile){
	fprintf(outputExcFile, "STARTHEADER\n");
	fprintf(outputExcFile,"\n");

	for(int i = 0; i<symbolCount; i++){
		fprintf(outputExcFile, "%s $%04x\n", symbolTable[i]->name, symbolTable[i]->address);
	}
	fprintf(outputExcFile,"\n");

	fprintf(outputExcFile, "ENDHEADER\n");
	fprintf(outputExcFile,"\n");


}

// Look up symbol tables with given strings.
// If string correspond to a symbol, return the address in string form
// If not, return the original string
char* replaceSymbol(char* operand){
	char* hexAddress = (char*)malloc(sizeof(char)*5);
	for(int i =0; i<symbolCount; i++){
		char* name = symbolTable[i]->name;
		if(!strcmp(name,operand)){
			sprintf(hexAddress,"$%04x",symbolTable[i]->address);
			return hexAddress;
		}
	}
	return operand;
}

// Go through list of operands, replace all symbols with addresses
void updateSymbolOperand(StringList* operandList){
	char** list = operandList->stringArray;
	for(int i = 0; i<operandList->size;i++){
		char* operand = *(list+i);
		*(list+i) = replaceSymbol(operand);
	}
}

// Proces all symbols if present in a line 
char* processSymbol(char* line,int address){
	int length = strlen(line);
	char copy[length];
	strcpy(copy,line);
	removeSpaces(copy);

	char* split[2];
	split[0] = malloc(sizeof(char)*length);


	// If there is a symbol, split[0] will store symbol
	// Else split[0] will still store whole string
	strcpy(split[0],strtok(copy, ":"));

	
	//Return pointer to -1 if line only has a symbol 
	
	if(line[length-1] == ':'){
		updateSymbol(split[0],address);
		return (char*)-1;
	}

	split[1] = malloc(sizeof(char)*length);

	// If there is a symbol, split[1] has rest of line
	// Then return split[1]

	if(strlen(split[0]) < length){
		updateSymbol(split[0],address);

		strcpy(split[1],strtok(NULL, ":"));

		return split[1];

	}

	return split[0];			// Return all line if no symbol
}

