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


// Check if string has parentheses at start and end;
bool checkParenthesis(char* source){
	char start = *source;
	int length = strlen(source);
	char last = *(source + length - 1);

	if(start == '(' &&  last == ')'){
		return true;
	}
	return false;
}

// Convert a number to its binary string representation
char* intTo8BitBinary(int number){
	char* binaryNum = (char*)malloc(sizeof(int)*8); 

    for (int bitPosition = 7; bitPosition >= 0; bitPosition--)
    {
        int bitDigit = number >> bitPosition;

        if (bitDigit & 1)
          *(binaryNum + 7 - bitPosition) = '1';
        else
          *(binaryNum + 7 - bitPosition) = '0';
    }

    return binaryNum;
}


// Convert a string to uppercase
void toUpperCase(char* string){
	int index = 0;
	while(string[index]){
		string[index] = (char)toupper(string[index]);
		index++;
	}
}

//Remove all white spaces from a string
void removeSpaces(char* source){
	char* result = source;
	char* current = source;
	while(*current != 0)
	{
		*result = *current++;
		if(*result != ' ')
			result++;
	}
	*result = 0;
}

