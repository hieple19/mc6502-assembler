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


int currentAddress;

// Get all operands and store in a string list struct from an Operand Line
// String list struct has an array of strings and a size count

StringList* getOperands(char* operands){
	StringList* list = (StringList*)malloc(sizeof(StringList));
	int length = strlen(operands);
	char* copy= (char *)(malloc(sizeof(char)*length));
	strcpy(copy,operands);

	char** res = (char **)(malloc(sizeof(char)*length));
	int i=0;

	*res = strtok(copy,",");

	while(*(res+i) != 0){
		i++;
		*(res+i) = strtok(0,",");
	}

	list->stringArray = res;
	list->size = i;

	return list;
}

// First pass to read all symbols and store in a symbol table 
void firstPass(FILE *inputFile, int address){
	char line[200];

	while(fgets(line, sizeof(line),inputFile)){
		strtok(line, ";");				// Remove comments
		removeSpaces(line);

		line[strcspn(line, "\n")] = 0;	// Remove whitespaces and end of line symbol
		
		if(strcmp(line,"")){			// Check that line is not null

			char* operation = processSymbol(line,address);	// After processing, operation variable may hold the non-symbol token of the line. 
															// Or null if line contains only a symbol

			if(operation != (char*)-1){					// If line has a non-symbol token 
				char opcode[3];							
				strncpy(opcode, operation,3);
				opcode[3] = 0;
				operation+=3;							// Get opcode from string

				toUpperCase(opcode);
				StringList* list = getOperands(operation);			// Get list of operands from operation line
				address = address + calculateBytes(opcode,list);	// Update address of line 
			}
		}
	}
}

void secondPass(FILE *inputFile, FILE *outputExcFile, FILE *outputPrtFile, int address){
	char line[200];
	printSymbols(outputExcFile);

	while(fgets(line, sizeof(line),inputFile)){
		strtok(line, ";");
		removeSpaces(line);
		line[strcspn(line, "\n")] = 0;						// Remove whitespaces and end of line symbol

		if(strcmp(line,"")){
			char* operation = processSymbol(line,address);	// After processing, operation variable may hold the non-symbol token of the line. 
															// Or null if line contains only a symbol			

			if(operation != (char*)-1){
				printf("$%04x ", currentAddress);			// Print current address

				char opcode[3];					
				strncpy(opcode, operation,3);
				opcode[3] = 0;
				operation+=3;

				toUpperCase(opcode);							// Get upper case opcode

				StringList* list = getOperands(operation);		// Get operands from line

				int byteLength = calculateBytes(opcode,list);	// Calculate number of bytes of line
				int hexOpCode = opCodeHex(opcode,list);

				int* hexCode = processOperands(opcode,list);	// Get hexOpCode

				printf(" %02x ", hexOpCode);
				fprintf(outputPrtFile, "%02x ", hexOpCode);						// Print hex op codes to terminal and files
				fprintf(outputExcFile, "%s", intTo8BitBinary(hexOpCode));
				
				int count = 0;

				// Print all operands represenation till reaches -1 value (terminating value)
				while(*(hexCode+count) != -1)
				{
					printf("%02x ", *(hexCode + count) & 0xff);
					fprintf(outputPrtFile, "%02x ", *(hexCode + count) & 0xff);
					fprintf(outputExcFile, "%s", intTo8BitBinary(*(hexCode + count)));

					count++;
				}
				fprintf(outputPrtFile, "\n");
				printf("\n");

				currentAddress = currentAddress + byteLength;					// Update address
			}
		}
	}
}
