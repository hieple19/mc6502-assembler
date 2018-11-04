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

// Accumulator has form: A or no operand
bool checkAccumulator(StringList* operandList){
	if(operandList->size == 1){
		return (char)*(*(operandList->stringArray)) == 'A';
	}
	else if(operandList->size == 0){
		return true;
	}
	return false;
}

// Indirect has form (SYMBOL)
bool checkIndirect(StringList* operandList){
	if(operandList->size == 1){
		char** operands = operandList->stringArray;
		char* operand1 = operands[0];
		return checkParenthesis(operand1);
	}
	return false;
}

// Indirect X has form (SYMBOL, X)  
bool checkIndirectX(StringList* operandList){
	if(operandList->size == 2){
		char** operands = operandList->stringArray;
		char* first = operands[0];
		char start = *first;
		char* operand2 = operands[1];
		int length = strlen(operand2);
		char last = *(operand2+length-1);

		// Check first operand starts with (, second ends with )
		if(start == '(' &&  last == ')' && strlen(first) == 4){
			return true;
		}
		return false;
	}
	return false;
}

// Indirect Y has form (SYMBOL),Y
bool checkIndirectY(StringList* operandList){
	if(operandList->size == 2){
		char** operands = operandList->stringArray;
		char* operand1 = operands[0];
		if(checkParenthesis(operand1)){
			char* operand2 = operands[1];
			char letter = (char) *operand2;
			if(letter == 'Y'){
				return true;
			}
		}
		return false; 
	}
	return false;
}

// Imediate starts with #
bool checkImmediate(StringList* operandList){
	if(operandList->size == 1){
		return (char)*(*(operandList->stringArray)) == '#';
	}
	return false;
}

// Zeropage starts with $ and has 2 digits
bool checkZeroPage(StringList* operandList){
	if(operandList->size == 1){
		char* operand = operandList->stringArray[0];
		if((char)*operand == '$'){
			return strlen(operand) == 3;
		}
	}
	return false;
}

// ZeropageX has zero page and second operand is X
bool checkZeroPageX(StringList* operandList){
	if(operandList->size == 2){
		char** operands = operandList->stringArray;
		char* operand1 = operands[0];
		if((char)*operand1 == '$' && strlen(operand1)==3){
			if((char)*(operands[1]) == 'X'){
				return true;
			}
		}
	}
	return false;
}

// ZeropageX has zero page and second operand is Y
bool checkZeroPageY(StringList* operandList){
	if(operandList->size == 2){
		char** operands = operandList->stringArray;
		char* operand1 = operands[0];
		if((char)*operand1 == '$' && strlen(operand1)==3){
			if((char)*(operands[1]) == 'Y'){
				return true;
			}
		}
	}
	return false;
}

// Absolute starts with $ and has 4 digits
bool checkAbsolute(StringList* operandList){
	if(operandList->size == 1){
		char* operand = operandList->stringArray[0];
		if((char)*operand == '$'){
			return strlen(operand) == 5;
		} else{
			return true;
		}
	}
	return false;
}

// AbsoluteX has X as second operand
bool checkAbsoluteX(StringList* operandList){
	if(operandList->size == 2){
		char** operands = operandList->stringArray;
		if((char)*operands[1] == 'X'){
			return true;
		}
		// }
	}
	return false;
}

// AbsoluteY has Y as second operand
bool checkAbsoluteY(StringList* operandList){
	if(operandList->size == 2){
		char** operands = operandList->stringArray;
		if((char)*operands[1] == 'Y'){
			return true;
		}
	}
	return false;
}

// If opcode meets one of 8 branch codes
bool checkBranching(char* opCode, StringList* operandList){
	char* branchCodes[8] = {"BPL", "BMI","BVC","BVS","BCC","BCS","BNE","BEQ"};
	if(operandList->size == 1){
		for(int i = 0; i<8; i++){
			if(!strcmp(opCode, branchCodes[i])){
				return true;
			}
		}
	}
	return false;
}

bool (*ftable[11])(StringList* operandList);

void initializeCheckTable(){
	ftable[IMMEDIATE] = checkImmediate;  		// Function table for 
	ftable[ZEROPAGE] = checkZeroPage;
	ftable[ZEROPAGEX] = checkZeroPageX;
	ftable[ZEROPAGEY] = checkZeroPageY;
	ftable[INDIRECT] = checkIndirect;
	ftable[INDIRECTX] = checkIndirectX;
	ftable[INDIRECTY] = checkIndirectY;
	ftable[ACCUMULATOR] = checkAccumulator;
	ftable[ABSOLUTE] = checkAbsolute;
	ftable[ABSOLUTEX] = checkAbsoluteX;
	ftable[ABSOLUTEY] = checkAbsoluteY;
}

// Return type order based on the OpCode.txt file
int checkType (char* opCode, StringList* operandList){
	if(checkBranching(opCode, operandList)){
		return 11;
	}

	for(int i = 0; i<11; i++){
		if((*ftable[i])(operandList)){
			return i;						
		}
	}

	return -1;
}

// Return bytes number based on type of operands
int calculateBytes(char* opCode, StringList* operandList){
	int operandType = checkType(opCode, operandList);
	if(operandType == ACCUMULATOR){
		return 1;
	} else if(operandType == IMMEDIATE || operandType == ZEROPAGE || operandType == ZEROPAGEX || operandType == ZEROPAGEY || operandType == INDIRECTX || operandType == INDIRECTY || operandType == BRANCH){
		return 2;
	} else if(operandType == ABSOLUTE || operandType == ABSOLUTEX || operandType == ABSOLUTEY || operandType == INDIRECT){
		return 3;
	} else{
		return 0 ;
	}
}
