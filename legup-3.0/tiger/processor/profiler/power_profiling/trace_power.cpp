#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

// Group A: j,bltz,blez,bgtz,bltzal,sltiu,bgezal,bgez,jal,lui	nop,mfhi,mflo
// Group B: slti,sra,srl,sll,andi
// Group C: sw,sb,xori,or,addiu,addi
// Group D: sltu,slt,ori,div,srlv,and,xor,addu,sllv,add,mult,divu,sub,multu,subu
// Group E: bne,beq												lbu
// Group F: lb,lw

#define op2(a,b) ((_op & (long int)(pow(2,a+1)-1))>>b)
#define op(a) ((_op & (long int)pow(2,a))>>(a))
#define instr2(a,b) ((_instr & (long int)(pow(2,a+1)-1))>>b)
#define instr(a) ((_instr & (long int)pow(2,a))>>(a))
#define funct2(a,b) ((_funct & (long int)(pow(2,a+1)-1))>>b)
#define funct(a) ((_funct & (long int)pow(2,a))>>(a))

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define UNKNOWN 6
#define ERROR -1


// LB: 1000_0000_0100_0100___0000_0000_0000_0001
//	   1000 00ss ssst tttt   iiii iiii iiii iiii

int get_group (int _instr) {
	int _op = instr2(31,26);
	int _funct = instr2(5,0);

	// NOP
	if (_instr == 0) {
		return A;
	
	// BEQ, BNE, BLEZ, BGTZ
	} else if (op2(5,2) == 0b0001) {
		if (op(1) == 0) return E;	// BEQ, BNE
		else			return A;	// BLEZ, BGTZ, BGEZAL
		
	// BLTZ, BGEZ, BGEZAL, BLTZAL
	} else if (_op == 0b000001) {
		return A;

	// R-type instruction
	} else {
		if (_op == 0b000000) {
			// ADD, ADDU, SUB, SUBU
			if (funct2(5,2) == 0b1000) {
				return D;
			
			// MUL, MULU, DIV, DIVU
			} else if (funct2(5,2) == 0b0110) {
				return D;	// was blank, why?
			
			// AND, OR, XOR
			} else if (funct2(5,2) == 0b1001) {
				if (funct(0) == 0) return D;	// AND, XOR
				else			   return C;	// OR
			
			// MFHI, MFLO, 
			} else if (funct2(5,2) == 0b0100) {
				return A;
				
			// SLL, SRA, SRL, SLLV, SRLV
			} else if (funct2(5,3) == 0b000 && (funct(1) || !funct(0))) {
				if (funct(2) == 0) return B;	// SLL, SRA, SRL
				else 			   return D;	// SLLV, SRLV

			// SLT, SLTU
			} else if (funct2(5,1) == 0b10101) {
				return D;
			
			// JR
			} else if (funct2(5,0) == 0b001000) {
				return UNKNOWN;
				
			} else {
				return ERROR;
			}

		// ADDI, ADDIU, SLTI, SLTIU, ANDI, ORI, XORI, LUI
		} else if (op2(5,3) == 0b001) {
			if (!op(0) && (op(1) ^ op(2))) 	return B; // SLTI, ANDI 
			else if (op2(1,0) == 0b11) 		return A; // SLTIU, LUI
			else if (op2(2,0) == 0b101) 		return D; // ORI 
			else 							return C; // ADDI, ADDIU, XORI
			
		// LBU?
		} else if (_op == 0b100100) {
			return E;
			
		// LB, LW
		} else if (op2(5,3) == 0b100 && (!op(2) | !op(1) | !op(0))) {
			return F;
			
		// SB, SW
		} else if (op2(5,2) == 0b1010 && (!op(1) | op(0))) {
			return C;
			
		// J, JAL
		} else if (op2(5,1) == 0b00001) {
			return A;

		} else {
			printf("not found! ERROR\n");
			return ERROR;
		}
	}
}

int main(int argc, char** argv) {
	if (argc != 4) { printf("Incorrect usage.  Expecting <instr_trace_file> <pc_trace_file>.\n"); exit(1); }
	
	FILE* trace = fopen(argv[1], "r");
	FILE* pc_trace = fopen(argv[2], "r");
	ifstream asm_trace; asm_trace.open(argv[3], ios::in);
	int instr, pc;
	int counters[6];
	int c = 1;
	string line;

	
	while (trace != NULL) {
		if (fscanf(trace, "%x\n", &instr) == -1) break;
		fscanf(pc_trace, "%x\n", &pc);
		getline(asm_trace, line);
		// remove [..]
		int x;
		if ((x=line.find_first_of("[")) != string::npos) line = line.substr(0,x);
		if ((x=line.find_first_of("<")) != string::npos) line = line.substr(0,x);
		
		//counters[get_group(instr)]++;
		//printf("%6d: %08x  %08x --> %25s --> ", c, pc, instr, line.c_str());
		switch (get_group(instr)) {
			case A: printf("A\n"); break;
			case B: printf("B\n"); break;
			case C: printf("C\n"); break;
			case D: printf("D\n"); break;
			case E: printf("E\n"); break;
			case F: printf("F\n"); break;
			case UNKNOWN: printf("UNKNOWN\n"); break;
			case ERROR: printf("ERR\n"); break;
			default: printf("default case?\n"); break;
		}
		c++;
		//exit(1);
	}
	
	for (int i=0; i<6; i++) {
	//	printf("counter[%d] = %d\n", i, counters[i]);
	}
	return 0;
}
