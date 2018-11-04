typedef struct Symbol{
	char* name;
	int address;
} Symbol;

extern Symbol* symbolTable[];
extern int symbolCount;
extern int currentAddress;

extern void updateSymbolOperand(StringList* operandList);
extern char* replaceSymbol(char* operand);
extern void updateSymbol(char* name, int address);
extern char* processSymbol(char* line,int address);
extern void printSymbols(FILE* outputExcFile);