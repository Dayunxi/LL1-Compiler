#ifndef __LAB8_GLOBAL__
#define __LAB8_GLOBAL__ 
enum{		//32 terminals		//�ս����1��ʼ������ʷ������ķ���ֵ��ͻ 
	IF=1, ELSE, WHILE, DO, BREAK, TRUE, FALSE, INT, BOOL, AND, OR, NOT, EQ, NE, GT, GE, LT, LE, SET, ADD, SUB, MUL, DIV, SC, LP, RP, LB, RB, LSB, RSB, ID, NUM
};
enum{		// non-terminals & auxiliaries
	PRO, BLO, DECLS, DECL, TYP, TYPP, BASIC, STMTS, STMT, LOC, LOCP, BEXP, BEXPP, JOIN, JOINP, EQU, EQUP, REL, RELP, AEXP, AEXPP, TERM, TERMP, UNY, FAC, EPS, END
};
enum{		// Operator
	opASSIGN, opADD, opSUB, opMUL, opDIV, opARRGETLEFT, opARRGETRIGHT, opAND, opOR, opNOT, opLT, opEQ, opNE, opGT, opLE, opGE, opJMP, opJNE, opJE, opJZ, opJNZ, opJL, opJG, opJLE, opJGE
};
//opARRGETLEFT -> arr[index] = n.place  		opARRGETRIGHT -> n.place = arr[index]
enum{		// ���������� 
	NONE, IMMEDIATE, VARIABLE, ADDRESS
};
#endif
