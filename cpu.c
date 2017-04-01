#include <stdio.h>
#include <stdlib.h>

#include "registers.h"
#include "cpu.h"
#include "memory.h"

const struct instruction instructions[256] = {
	{ "NOP", 0, 2, nop},                           // 0x00
	{ "LD BC, 0x%04X", 2, 6, undefined},            // 0x01
	{ "LD (BC), A", 0, 4, undefined },               // 0x02
	{ "INC BC", 0, 4, undefined },                     // 0x03
	{ "INC B", 0, 2, undefined },                       // 0x04
	{ "DEC B", 0, 2, dec_b },                       // 0x05
	{ "LD B, 0x%02X", 1, 4, ldb_n },               // 0x06
	{ "RLCA", 0, 4, undefined },                         // 0x07
	{ "LD (0x%04X), SP", 2, 10, undefined },         // 0x08
	{ "ADD HL, BC", 0, 4, undefined },              // 0x09
	{ "LD A, (BC)", 0, 4, undefined},               // 0x0a
	{ "DEC BC", 0, 4, undefined },                     // 0x0b
	{ "INC C", 0, 2, undefined },                       // 0x0c
	{ "DEC C", 0, 2, dec_c },                       // 0x0d
	{ "LD C, 0x%02X", 1, 4, ldc_n },               // 0x0e
	{ "RRCA", 0, 4, undefined },                         // 0x0f
	{ "STOP", 1, 2, undefined },                         // 0x10
	{ "LD DE, 0x%04X", 2, 6, undefined },            // 0x11
	{ "LD (DE), A", 0, 4, undefined },               // 0x12
	{ "INC DE", 0, 4, undefined },                     // 0x13
	{ "INC D", 0, 2, undefined },                       // 0x14
	{ "DEC D", 0, 2, undefined},                       // 0x15
	{ "LD D, 0x%02X", 1, 4, undefined },               // 0x16
	{ "RLA", 0, 4, undefined },                           // 0x17
	{ "JR 0x%02X", 1, 4, undefined },                    // 0x18
	{ "ADD HL, DE", 0, 4, undefined },              // 0x19
	{ "LD A, (DE)", 0, 4, undefined },               // 0x1a
	{ "DEC DE", 0, 4, undefined },                     // 0x1b
	{ "INC E", 0, 2, undefined },                       // 0x1c
	{ "DEC E", 0, 2, undefined },                       // 0x1d
	{ "LD E, 0x%02X", 1, 4, undefined },               // 0x1e
	{ "RRA", 0, 4, undefined },                           // 0x1f
	{ "JR NZ, 0x%02X", 1, 0, jr_nz_n },             // 0x20
	{ "LD HL, 0x%04X", 2, 6, ldhl_nn },            // 0x21
	{ "LDI (HL), A", 0, 4, undefined },             // 0x22
	{ "INC HL", 0, 4, undefined },                     // 0x23
	{ "INC H", 0, 2, undefined },                       // 0x24
	{ "DEC H", 0, 2, undefined},                       // 0x25
	{ "LD H, 0x%02X", 1, 4, undefined },               // 0x26
	{ "DAA", 0, 2, undefined},                           // 0x27
	{ "JR Z, 0x%02X", 1, 0, undefined},               // 0x28
	{ "ADD HL, HL", 0, 4, undefined },              // 0x29
	{ "LDI A, (HL)", 0, 4, undefined },             // 0x2a
	{ "DEC HL", 0, 4, undefined },                     // 0x2b
	{ "INC L", 0, 2, undefined },                       // 0x2c
	{ "DEC L", 0, 2, undefined },                       // 0x2d
	{ "LD L, 0x%02X", 1, 4, undefined },               // 0x2e
	{ "CPL", 0, 2, undefined },                           // 0x2f
	{ "JR NC, 0x%02X", 1, 4, undefined },             // 0x30
	{ "LD SP, 0x%04X", 2, 6, undefined },            // 0x31
	{ "LDD (HL), A", 0, 4, ldd_phl_a },             // 0x32
	{ "INC SP", 0, 4, undefined },                     // 0x33
	{ "INC (HL)", 0, 6, undefined },                  // 0x34
	{ "DEC (HL)", 0, 6, undefined },                  // 0x35
	{ "LD (HL), 0x%02X", 1, 6, undefined },          // 0x36
	{ "SCF", 0, 2, undefined },                           // 0x37
	{ "JR C, 0x%02X", 1, 0, undefined },               // 0x38
	{ "ADD HL, SP", 0, 4, undefined },              // 0x39
	{ "LDD A, (HL)", 0, 4, undefined },             // 0x3a
	{ "DEC SP", 0, 4, undefined },                     // 0x3b
	{ "INC A", 0, 2, undefined},                       // 0x3c
	{ "DEC A", 0, 2, undefined },                       // 0x3d
	{ "LD A, 0x%02X", 1, 4, lda_n },               // 0x3e
	{ "CCF", 0, 2, undefined },                           // 0x3f
	{ "LD B, B", 0, 2, undefined },                       // 0x40
	{ "LD B, C", 0, 2, undefined },                    // 0x41
	{ "LD B, D", 0, 2, undefined },                    // 0x42
	{ "LD B, E", 0, 2, undefined },                    // 0x43
	{ "LD B, H", 0, 2, undefined },                    // 0x44
	{ "LD B, L", 0, 2, undefined },                    // 0x45
	{ "LD B, (HL)", 0, 4, undefined },               // 0x46
	{ "LD B, A", 0, 2, undefined },                    // 0x47
	{ "LD C, B", 0, 2, undefined },                    // 0x48
	{ "LD C, C", 0, 2, undefined },                       // 0x49
	{ "LD C, D", 0, 2, undefined },                    // 0x4a
	{ "LD C, E", 0, 2, undefined },                    // 0x4b
	{ "LD C, H", 0, 2, undefined },                    // 0x4c
	{ "LD C, L", 0, 2, undefined },                    // 0x4d
	{ "LD C, (HL)", 0, 4, undefined },               // 0x4e
	{ "LD C, A", 0, 2, undefined },                    // 0x4f
	{ "LD D, B", 0, 2, undefined },                    // 0x50
	{ "LD D, C", 0, 2, undefined },                    // 0x51
	{ "LD D, D", 0, 2, undefined },                       // 0x52
	{ "LD D, E", 0, 2, undefined },                    // 0x53
	{ "LD D, H", 0, 2, undefined },                    // 0x54
	{ "LD D, L", 0, 2, undefined },                    // 0x55
	{ "LD D, (HL)", 0, 4, undefined },               // 0x56
	{ "LD D, A", 0, 2, undefined},                    // 0x57
	{ "LD E, B", 0, 2, undefined },                    // 0x58
	{ "LD E, C", 0, 2, undefined },                    // 0x59
	{ "LD E, D", 0, 2, undefined },                    // 0x5a
	{ "LD E, E", 0, 2, undefined },                       // 0x5b
	{ "LD E, H", 0, 2, undefined },                    // 0x5c
	{ "LD E, L", 0, 2, undefined },                    // 0x5d
	{ "LD E, (HL)", 0, 4, undefined },               // 0x5e
	{ "LD E, A", 0, 2, undefined },                    // 0x5f
	{ "LD H, B", 0, 2, undefined },                    // 0x60
	{ "LD H, C", 0, 2, undefined },                    // 0x61
	{ "LD H, D", 0, 2, undefined },                    // 0x62
	{ "LD H, E", 0, 2, undefined },                    // 0x63
	{ "LD H, H", 0, 2, undefined },                       // 0x64
	{ "LD H, L", 0, 2, undefined },                    // 0x65
	{ "LD H, (HL)", 0, 4, undefined },               // 0x66
	{ "LD H, A", 0, 2, undefined },                    // 0x67
	{ "LD L, B", 0, 2, undefined },                    // 0x68
	{ "LD L, C", 0, 2, undefined },                    // 0x69
	{ "LD L, D", 0, 2, undefined },                    // 0x6a
	{ "LD L, E", 0, 2, undefined },                    // 0x6b
	{ "LD L, H", 0, 2, undefined },                    // 0x6c
	{ "LD L, L", 0, 2, undefined },                       // 0x6d
	{ "LD L, (HL)", 0, 4, undefined },               // 0x6e
	{ "LD L, A", 0, 2, undefined },                    // 0x6f
	{ "LD (HL), B", 0, 4, undefined },               // 0x70
	{ "LD (HL), C", 0, 4, undefined },               // 0x71
	{ "LD (HL), D", 0, 4, undefined },               // 0x72
	{ "LD (HL), E", 0, 4, undefined },               // 0x73
	{ "LD (HL), H", 0, 4, undefined },               // 0x74
	{ "LD (HL), L", 0, 4, undefined },               // 0x75
	{ "HALT", 0, 2, undefined },                         // 0x76
	{ "LD (HL), A", 0, 4, undefined },               // 0x77
	{ "LD A, B", 0, 2, undefined },                    // 0x78
	{ "LD A, C", 0, 2, undefined },                    // 0x79
	{ "LD A, D", 0, 2, undefined },                    // 0x7a
	{ "LD A, E", 0, 2, undefined },                    // 0x7b
	{ "LD A, H", 0, 2, undefined },                    // 0x7c
	{ "LD A, L", 0, 2, undefined },                    // 0x7d
	{ "LD A, (HL)", 0, 4, undefined },               // 0x7e
	{ "LD A, A", 0, 2, undefined },                       // 0x7f
	{ "ADD A, B", 0, 2, undefined },                  // 0x80
	{ "ADD A, C", 0, 2, undefined },                  // 0x81
	{ "ADD A, D", 0, 2, undefined },                  // 0x82
	{ "ADD A, E", 0, 2, undefined },                  // 0x83
	{ "ADD A, H", 0, 2, undefined },                  // 0x84
	{ "ADD A, L", 0, 2, undefined },                  // 0x85
	{ "ADD A, (HL)", 0, 4, undefined },             // 0x86
	{ "ADD A", 0, 2, undefined },                     // 0x87
	{ "ADC B", 0, 2, undefined },                       // 0x88
	{ "ADC C", 0, 2, undefined },                       // 0x89
	{ "ADC D", 0, 2, undefined },                       // 0x8a
	{ "ADC E", 0, 2, undefined },                       // 0x8b
	{ "ADC H", 0, 2, undefined },                       // 0x8c
	{ "ADC L", 0, 2, undefined },                       // 0x8d
	{ "ADC (HL)", 0, 4, undefined },                  // 0x8e
	{ "ADC A", 0, 2, undefined },                       // 0x8f
	{ "SUB B", 0, 2, undefined },                       // 0x90
	{ "SUB C", 0, 2, undefined },                       // 0x91
	{ "SUB D", 0, 2, undefined },                       // 0x92
	{ "SUB E", 0, 2, undefined },                       // 0x93
	{ "SUB H", 0, 2, undefined },                       // 0x94
	{ "SUB L", 0, 2, undefined },                       // 0x95
	{ "SUB (HL)", 0, 4, undefined },                  // 0x96
	{ "SUB A", 0, 2, undefined },                       // 0x97
	{ "SBC B", 0, 2, undefined },                       // 0x98
	{ "SBC C", 0, 2, undefined },                       // 0x99
	{ "SBC D", 0, 2, undefined },                       // 0x9a
	{ "SBC E", 0, 2, undefined },                       // 0x9b
	{ "SBC H", 0, 2, undefined },                       // 0x9c
	{ "SBC L", 0, 2, undefined },                       // 0x9d
	{ "SBC (HL)", 0, 4, undefined },                  // 0x9e
	{ "SBC A", 0, 2, undefined },                       // 0x9f
	{ "AND B", 0, 2, undefined },                       // 0xa0
	{ "AND C", 0, 2, undefined },                       // 0xa1
	{ "AND D", 0, 2, undefined },                       // 0xa2
	{ "AND E", 0, 2, undefined },                       // 0xa3
	{ "AND H", 0, 2, undefined },                       // 0xa4
	{ "AND L", 0, 2, undefined },                       // 0xa5
	{ "AND (HL)", 0, 4, undefined },                  // 0xa6
	{ "AND A", 0, 2, undefined },                       // 0xa7
	{ "XOR B", 0, 2, undefined },                       // 0xa8
	{ "XOR C", 0, 2, undefined },                       // 0xa9
	{ "XOR D", 0, 2, undefined },                       // 0xaa
	{ "XOR E", 0, 2, undefined },                       // 0xab
	{ "XOR H", 0, 2, undefined },                       // 0xac
	{ "XOR L", 0, 2, undefined },                       // 0xad
	{ "XOR (HL)", 0, 4, undefined },                  // 0xae
	{ "XOR A", 0, 2, xor_a },                       // 0xaf
	{ "OR B", 0, 2, undefined },                         // 0xb0
	{ "OR C", 0, 2, undefined },                         // 0xb1
	{ "OR D", 0, 2, undefined },                         // 0xb2
	{ "OR E", 0, 2, undefined },                         // 0xb3
	{ "OR H", 0, 2, undefined },                         // 0xb4
	{ "OR L", 0, 2, undefined },                         // 0xb5
	{ "OR (HL)", 0, 4, undefined },                    // 0xb6
	{ "OR A", 0, 2, undefined },                         // 0xb7
	{ "CP B", 0, 2, undefined },                         // 0xb8
	{ "CP C", 0, 2, undefined },                         // 0xb9
	{ "CP D", 0, 2, undefined },                         // 0xba
	{ "CP E", 0, 2, undefined },                         // 0xbb
	{ "CP H", 0, 2, undefined },                         // 0xbc
	{ "CP L", 0, 2, undefined },                         // 0xbd
	{ "CP (HL)", 0, 4, undefined },                    // 0xbe
	{ "CP A", 0, 2, undefined },                         // 0xbf
	{ "RET NZ", 0, 0, undefined },                     // 0xc0
	{ "POP BC", 0, 6, undefined },                     // 0xc1
	{ "JP NZ, 0x%04X", 2, 0, undefined },            // 0xc2
	{ "JP 0x%04X", 2, 8, jp },                   // 0xc3
	{ "CALL NZ, 0x%04X", 2, 4, undefined },        // 0xc4
	{ "PUSH BC", 0, 8, undefined },                   // 0xc5
	{ "ADD A, 0x%02X", 1, 4, undefined },             // 0xc6
	{ "RST 0x00", 0, 8, undefined },                    // 0xc7
	{ "RET Z", 0, 0, undefined },                       // 0xc8
	{ "RET", 0, 2, undefined },                           // 0xc9
	{ "JP Z, 0x%04X", 2, 0, undefined },              // 0xca
	{ "CB %02X", 1, 0, undefined },                      // 0xcb
	{ "CALL Z, 0x%04X", 2, 0, undefined },          // 0xcc
	{ "CALL 0x%04X", 2, 6, undefined },               // 0xcd
	{ "ADC 0x%02X", 1, 4, undefined },                  // 0xce
	{ "RST 0x08", 0, 8, undefined },                   // 0xcf
	{ "RET NC", 0, 0, undefined },                     // 0xd0
	{ "POP DE", 0, 6, undefined },                     // 0xd1
	{ "JP NC, 0x%04X", 2, 0, undefined },            // 0xd2
	{ "UNKNOWN", 0, 6, undefined },                 // 0xd3
	{ "CALL NC, 0x%04X", 2, 0, undefined },        // 0xd4
	{ "PUSH DE", 0, 8, undefined },                   // 0xd5
	{ "SUB 0x%02X", 1, 4, undefined },                  // 0xd6
	{ "RST 0x10", 0, 8, undefined },                   // 0xd7
	{ "RET C", 0, 0, undefined },                       // 0xd8
	{ "RETI", 0, 8, undefined },          // 0xd9
	{ "JP C, 0x%04X", 2, 0, undefined },              // 0xda
	{ "UNKNOWN", 0, 0, undefined },                 // 0xdb
	{ "CALL C, 0x%04X", 2, 0, undefined },          // 0xdc
	{ "UNKNOWN", 0, 0, undefined },                 // 0xdd
	{ "SBC 0x%02X", 1, 4, undefined },                  // 0xde
	{ "RST 0x18", 0, 8, undefined },                   // 0xdf
	{ "LD (0xFF00 + 0x%02X), A", 1, 6, undefined },// 0xe0
	{ "POP HL", 0, 6, undefined },                     // 0xe1
	{ "LD (0xFF00 + C), A", 0, 4, undefined },      // 0xe2
	{ "UNKNOWN", 0, 0, undefined },                 // 0xe3
	{ "UNKNOWN", 0, 0, undefined },                 // 0xe4
	{ "PUSH HL", 0, 8, undefined },                   // 0xe5
	{ "AND 0x%02X", 1, 4, undefined },                  // 0xe6
	{ "RST 0x20", 0, 8, undefined },                   // 0xe7
	{ "ADD SP,0x%02X", 1, 8, undefined },            // 0xe8
	{ "JP HL", 0, 2, undefined },                       // 0xe9
	{ "LD (0x%04X), A", 2, 8, undefined },           // 0xea
	{ "UNKNOWN", 0, 0, undefined },                 // 0xeb
	{ "UNKNOWN", 0, 0, undefined },                 // 0xec
	{ "UNKNOWN", 0, 0, undefined },                 // 0xed
	{ "XOR 0x%02X", 1, 4, undefined },                  // 0xee
	{ "RST 0x28", 0, 8, undefined },                   // 0xef
	{ "LD A, (0xFF00 + 0x%02X)", 1, 6, undefined },// 0xf0
	{ "POP AF", 0, 6, undefined },                     // 0xf1
	{ "LD A, (0xFF00 + C)", 0, 4, undefined },      // 0xf2
	{ "DI", 0, 2, di },                        // 0xf3
	{ "UNKNOWN", 0, 0, undefined },                 // 0xf4
	{ "PUSH AF", 0, 8, undefined },                   // 0xf5
	{ "OR 0x%02X", 1, 4, undefined },                    // 0xf6
	{ "RST 0x30", 0, 8, undefined },                   // 0xf7
	{ "LD HL, SP+0x%02X", 1, 6, undefined },       // 0xf8
	{ "LD SP, HL", 0, 4, undefined },                // 0xf9
	{ "LD A, (0x%04X)", 2, 8, undefined },           // 0xfa
	{ "EI", 0, 2, undefined },                             // 0xfb
	{ "UNKNOWN", 0, 0, undefined },                 // 0xfc
	{ "UNKNOWN", 0, 0, undefined },                 // 0xfd
	{ "CP 0x%02X", 1, 4, undefined },                    // 0xfe
	{ "RST 0x38", 0, 8, undefined },                   // 0xff
};

void setRegisterDefaults() {
	registers.af = 0x01B0;
	registers.bc = 0x0013;
	registers.de = 0x00D8;
	registers.hl = 0x014D;
}

void printRegisters() {
	printf("Registers:\n\t AF:%04X\n\t BC:%04X\n\t DE:%04X\n\t HL:%04X\n\n", registers.af, registers.bc, registers.de, registers.hl);
}


int main(int argc, char** argv) {
		
	FILE *f = fopen(argv[1], "rb");

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	fread(cartridge, 1, size, f);

	fclose(f);

	setRegisterDefaults();

	unsigned short operands = 0;
	registers.pc = 0x0100;
	struct instruction currentInstruction;

	while(1) {
		printf("PC: %x\n", registers.pc);
		currentInstruction = instructions[cartridge[registers.pc++]];

		printf("%s\n", currentInstruction.disassembly);

		if(currentInstruction.operandLength == 1) { operands = (unsigned short)cartridge[registers.pc]; }
		else if(currentInstruction.operandLength == 2) { operands = cartridge[registers.pc] | (cartridge[registers.pc + 1] << 8); }
		registers.pc += currentInstruction.operandLength;


		switch(currentInstruction.operandLength) {
			case 0:
				((void (*)(void))(currentInstruction.execute))();
				printRegisters();
				break;
			case 1:
				((void (*)(unsigned char))(currentInstruction.execute))((unsigned char)operands);
				printRegisters();
				break;
			case 2:
				((void (*)(unsigned short))(currentInstruction.execute))(operands);
				printRegisters();
				break;
		}
	}

	return 0;

}

void undefined() {
	struct instruction problem;

	printf("Undefined instruction: %s\n", problem.disassembly);
	printRegisters();

	getchar();
	return;
}

void nop() { }

void jp(unsigned short operand) { registers.pc = operand; }

void xor(unsigned char operand) { 

	registers.a ^= operand;
	if(operand == 0)
		SET(FLAGZ);
	else SET(FLAGZ);

	CLEAR(FLAGN | FLAGC | FLAGH);
}

void dec(unsigned char operand) {

	operand--;

	if(registers.b) SET(FLAGZ);
	else CLEAR(FLAGZ);
}

void xor_a() { xor(registers.a); }

void ldhl_nn(unsigned short operand) { registers.hl = operand; }

void ldc_n(unsigned char operand) { registers.c = operand; }

void ldb_n(unsigned char operand) { registers.b = operand; }

void lda_n(unsigned char operand) { registers.a = operand; }

void ldd_phl_a() { writeByte(registers.hl, registers.a); }

void dec_b() { dec(registers.b); }

void dec_c() { dec(registers.c); }

void jr_nz_n(unsigned char operand) {
	if(ISSET(FLAGZ))
		registers.pc += (signed char)operand;
}

void di() { registers.ie = 0; }