#include"src/global.h"
#include"src/table.h"
#include"src/semantic.h"
#include"src/common.h"
#include<stdio.h>
#include<list>
#include<vector>
#include<string>

using std::list;
using std::vector;
using std::string;

//语义变量 
Syntax *currSyntax = new Syntax();
//BoolControl *currBoolean = new BoolControl();
//ControlFlow *currControl = new ControlFlow();
BreakList *currBreakQuad = new BreakList();
vector<QuadTuple> sequence;
int currTempNum = 1;


//结构体方法 
Operand::Operand(){
	type = NONE;
	variable = NULL;
	address = 0;
}
Operand::Operand(TableElem *temp){
	type = VARIABLE;
	variable = temp;
	address = 0;
}
Operand::Operand(int n){
	type = IMMEDIATE;
	immediate = n;
	variable = NULL;
	address = 0;
}
Operand::Operand(int addr, bool isAddress){
	if(isAddress){
		type = ADDRESS;
		variable = NULL;
		address = addr;
	}
}

QuadTuple::QuadTuple(){}
QuadTuple::QuadTuple(int op_, Operand op1, Operand op2, Operand res){
	op = op_;
	operand1 = op1;
	operand2 = op2;
	result = res;
}

Syntax::Syntax(){
	place = offset = NULL;
	i = s = NULL;
	parent = NULL;
}
void Syntax::clear(){
	place = offset = NULL;
	i = s = NULL;
	parent = NULL;
}
Syntax* Syntax::derive(){		// 派生 继承 
	Syntax *newSyntax = new Syntax();
	newSyntax->parent = this;
	newSyntax->i = this->place;
	return newSyntax;
}

//BoolControl::BoolControl(){
//	parent = NULL;
//}
//BoolControl* BoolControl::derive(){
//	BoolControl *newBool = new BoolControl();
//	newBool->parent = this;
//	newBool->i_falselist = this->falselist;
//	newBool->i_truelist = this->truelist;
//	return newBool;
//}
//
//ControlFlow::ControlFlow(){
//	parent = NULL;
//}
//ControlFlow* ControlFlow::derive(){
//	ControlFlow *newControl = new ControlFlow();
//	newControl->parent = this;
//	newControl->i = this->nextlist;
//	return newControl;
//}
BreakList::BreakList(){
	parent = NULL;
}
BreakList* BreakList::derive(){
	BreakList *newlist = new BreakList();
	newlist->parent = this;
	newlist->i = this->breaklist;
	return newlist;
}


//语义分析函数 

//有操作数为空时传 new Operand() 才行 
QuadTuple* emit(int op, Operand *operand1, Operand *operand2, Operand *result){
	sequence.push_back(QuadTuple(op, *operand1, *operand2, *result));
	return &sequence.back();
}

//void emitNewTemp(int opType){		// 简化点 	
//	Operand *new_operand = newTemp(currType->typeCode);		//危险！！ 
//	emit(opType, currSyntax->i, currSyntax->place, new_operand);
//	currSyntax = currSyntax->derive();
//	currSyntax->i = new_operand;
//}

Operand* newTemp(int typecode){
	Type tempType;
	tempType.typeCode = typecode;
	char numStr[5];
	sprintf(numStr, "%d", currTempNum++);
	string newId = "T" + string(numStr);
	TableElem *new_temp = topTable->add(newId, tempType);
	return new Operand(new_temp);
}


void backpatch(list<int> jmplist, int quadIndex){
	for(list<int>::iterator iter=jmplist.begin(); iter!=jmplist.end(); iter++){
		sequence[*iter].result = Operand(quadIndex, true);
	}
}
void backpatch(int quad, int quadIndex){
	sequence[quad].result = Operand(quadIndex, true);
}

int nextQuad(){
	return sequence.size();
}


void printQuad(QuadTuple tuple){
	void printOperator(int opType);
	void printOperand(Operand operand);
	fprintf(stdout, "(");		
	printOperator(tuple.op);
	fprintf(stdout, ", ");
	printOperand(tuple.operand1);
	fprintf(stdout, ", ");
	printOperand(tuple.operand2);
	fprintf(stdout, ", ");
	printOperand(tuple.result);
	fprintf(stdout, ")");
}

void printOperator(int opType){
	switch(opType){
		case opADD:
			fprintf(stdout, "+");
			break;
		case opSUB:
			fprintf(stdout, "-");
			break;
		case opMUL:
			fprintf(stdout, "*");
			break;
		case opDIV:
			fprintf(stdout, "/");
			break;
		case opASSIGN:
			fprintf(stdout, "=");
			break;
		case opARRGETLEFT:		//形如 arr[i] = n; 
			fprintf(stdout, "[L]");
			break;
		case opARRGETRIGHT:		//形如 n = arr[i]; 
			fprintf(stdout, "[R]");
			break;
		case opAND:
			fprintf(stdout, "AND");
			break;
		case opOR:
			fprintf(stdout, "OR");
			break;
		case opNOT:
			fprintf(stdout, "NOT");
			break;
		case opJNE:			//jump if not euqal //两操作数不等则跳转 
			fprintf(stdout, "JNE");
			break;
		case opJE:		// jump if equal
			fprintf(stdout, "JE");
			break;
		case opJZ:		// jump if zero 
			fprintf(stdout, "JZ");
			break;
		case opJNZ:		// jump if not zero //操作数不为零则跳转 
			fprintf(stdout, "JNZ");
			break;
		case opJMP:		// jump
			fprintf(stdout, "JMP");
			break;
		case opJL:		// jump if less
			fprintf(stdout, "JL");
			break;
		case opJG:		// jump if greater
			fprintf(stdout, "JG");
			break;
		case opJLE:		//jump if less or equal
			fprintf(stdout, "JLE");
			break;
		case opJGE:		//jump if greater or equal
			fprintf(stdout, "JGE");
			break;
	}
}

void printOperand(Operand operand){
	if(operand.type==VARIABLE){
		fprintf(stdout, "%s", operand.variable->id.c_str());
	}
	else if(operand.type==ADDRESS){
		fprintf(stdout, "%d", operand.address);
	}
	else if(operand.type==IMMEDIATE){
		fprintf(stdout, "%d", operand.immediate);
	}
	else{
		fprintf(stdout, "_");
	}
}







