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

// Given type is branching, return the offset between addresses
int processBranching(StringList* operandList,char* opCode){
	char* operand1 = operandList->stringArray[0];
	operand1+=1;
	int gotoAddress = (int)strtol(operand1, NULL, 16);
	int afterAddress = calculateBytes(opCode, operandList) + currentAddress; // Address of PC after branching command
	return gotoAddress - afterAddress;
}

int processImmediate(StringList* operandList){
	char* operand1 = operandList->stringArray[0];

	operand1+=1;
	if(*operand1 == '$'){
		operand1 += 1;
		return (int)strtol(operand1, NULL, 16);		// Operand is in form #$. Remove first 2 characters. Return int value
	} else{
		return (int)strtol(operand1, NULL, 10);		// Operand is in form #. Remove first character and return int
	}	
}

int processZeroPage(StringList* operandList){
	char* operand1 = operandList->stringArray[0];	// Operand in form $xx. Remove first character and return int of the rest

	operand1+=1;
	return (int)strtol(operand1, NULL, 16);
}

int processIndirectY(StringList* operandList){
	char* operand1 = operandList->stringArray[0];

	operand1+=1;							//. Operand 1 in form (...)..  Remove ( and ) from first operand
	operand1[strlen(operand1)-1]=0;

	if(*operand1 == '$'){							// First operand in form $xxxx. Remove $ and return 
		operand1+=1;
	}
	else{
		updateSymbolOperand(operandList);			// First operand in form SYMBOL. Replace with address and return.
	}

	return (int)strtol(operand1, NULL, 16);
}

int processIndirectX(StringList* operandList){
	char* operand1 = operandList->stringArray[0];	// Operand 1 in type (XXX

	operand1+=1;
	updateSymbolOperand(operandList);				// Remove ( and check for symbols.
	return (int)strtol(operand1, NULL, 16);
}

// Process operand strings and return an array of hex op codes
int* processOperands(char* opCode, StringList* operandList){
	int* hexCode = (int*)malloc(sizeof(int)*3); 

	if(checkAccumulator(operandList)){
		*hexCode = -1;					// -1 signifies the end of the hex code array. Used for printing purposes. 
		return hexCode;
	}
	updateSymbolOperand(operandList);

	
	int count = 1;						// Op code count. Default is 1. 

	if(checkBranching(opCode, operandList)){
		*hexCode = processBranching(operandList,opCode);
		
	}

	else if(checkImmediate(operandList)){
		*hexCode = processImmediate(operandList);
	}

	else if(checkZeroPage(operandList) || checkZeroPageX(operandList) || checkZeroPageY(operandList)){
		*hexCode = processZeroPage(operandList);
	}

	else if(checkIndirect(operandList) || checkIndirectY(operandList)){
		*hexCode = processIndirectY(operandList);
	}

	else if(checkIndirectX(operandList)){
		*hexCode = processIndirectX(operandList);
	}

	else if(checkAbsolute(operandList) || checkAbsoluteX(operandList) || checkAbsoluteY(operandList)){
		char* operand1 = operandList->stringArray[0];

		operand1+=1;
		int operandHex = (int)strtol(operand1, NULL, 16);	// Hex address in form $xxxx.

		int lowByte = operandHex & 0x0000FF;				// Split into low and high bytes.
		int highByte = (operandHex & 0x00FF00)>>8;

		*hexCode = lowByte;
		*(hexCode+1) = highByte;

		count = 2;
	}

	*(hexCode+count) = -1;
	return hexCode;

}