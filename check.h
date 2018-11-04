extern bool checkIndirectX(StringList* operandList);
extern bool checkIndirectY(StringList* operandList);
extern bool checkImmediate(StringList* operandList);
extern bool checkZeroPage(StringList* operandList);
extern bool checkZeroPageX(StringList* operandList);
extern bool checkZeroPageY(StringList* operandList);
extern bool checkAbsoluteX(StringList* operandList);
extern bool checkAbsoluteY(StringList* operandList);
extern bool checkAccumulator(StringList* operandList);
extern bool checkIndirect(StringList* operandList);
extern bool checkBranching(char* opCode, StringList* operandList);

extern int calculateBytes(char* opCode, StringList* operandList);
extern void initializeCheckTable();
extern bool checkAbsolute(StringList* operandList);
extern int checkType (char* opCode, StringList* operandList);

typedef enum{
	IMMEDIATE,
	ZEROPAGE,
	ZEROPAGEX,
	ZEROPAGEY,
	INDIRECT,
	INDIRECTX,
	INDIRECTY,
	ACCUMULATOR,
	ABSOLUTE,
	ABSOLUTEX,
	ABSOLUTEY,
	BRANCH
} AddressMode;