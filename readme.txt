Hiep Le - 3/11/2018
MC6502 - an 8-bit Two Pass Assembler

General Program Structure: 

	check.c 	- Check type of addressing modes and its byte length
	format.c 	- Format strings and numbers 
	opcode.c 	- Read operation codes and hex codes from opcode.txt
	process.c 	- Process operands into corresponding hex representations
	symbol.c 	- Read, construct and maintain symbol tables
	readin.c 	- Run two passes through two files

	main.c 		- I/O with files for reading and writing

Using Program
	
	Program takes in 2 arguments
		- The .asm file to assemble
		- Starting address of the assembled code

	In Command Line: 	./assembler <file.asm> <8-bit address in hex>
					e.g ./assembler conway.asm 0x0600

Program output
	- .prt file: Hex representation of the assembled code
	- .exc file: Symbol table and binary image of code

References
	
	http://skilldrick.github.io/easy6502/
	https://dwheeler.com/6502/oneelkruns/asm1step.html
	http://www.6502.org/tutorials/6502opcodes.html