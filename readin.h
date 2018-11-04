typedef struct StringList{
	char** stringArray;
	int size;
} StringList;

extern StringList* getOperands(char* operands);
extern void firstPass(FILE *inputFile, int address);
extern void secondPass(FILE *inputFile, FILE *outputExcFile, FILE *outputPrtFile, int address);
