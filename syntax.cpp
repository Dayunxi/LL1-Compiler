#include"src/global.h"
#include"src/word.h"
#include"src/syntax.h"
#include"src/semantic.h"
#include"src/common.h"
#include<string>
#include<vector>
#include<stdio.h>
#include<stdlib.h>

using std::string;
using std::vector;
using std::list;

//�ʷ��������� 
vector<WordUnit>::iterator sym;
//���ű���� 
Type *checkingType = new Type();	//�������ڱ����ı��������� 
Type *currArray = new Type(); 	//ָ��ǰ����������������� 
bool hasIdError = false;
bool isArrayInLeft = false; 	//�����ڸ�ֵ���ʽ����򲻱ؽ�ָ����ָ��ֵ�����±��� 
//bool isBoolControl = false; //�Ƿ����ڷ��벼��������� 


//�ݹ��½��������� 
void program(){
	block();
}

void block(){		//ÿ��Block����������ӷ��ű� 
	Table *newTable = new Table();
	newTable->parent = topTable;
	if(topTable) topTable->addChild(newTable);
	topTable = newTable;
	if(*sym==LSB) sym++; 
	else error(LSB);
	decls();
	stmts();
	if(*sym==RSB) sym++;
	else error(RSB);
	if(topTable->parent) topTable = topTable->parent;
}

void decls(){
	if(*sym!=INT&&*sym!=BOOL){
		return;
	}
	decl();
	decls();
}

void decl(){
	currType->clear();
	type();
	if(*sym==ID){
		topTable->add(sym->value, *currType);
		sym++;
	}
	else error(ID);
	if(*sym==SC) sym++;
	else error(SC);	
}

void type(){
	basic();
	typep();
}

void typep(){
	if(*sym!=LB){
		return;
	}
	if(*sym==LB) sym++;  
	if(*sym==NUM){
		currType = currType->derive(atoi(sym->value.c_str()));
		sym++;
	}
	else error(NUM);
	if(*sym==RB) sym++;
	else error(RB);
	typep();
	currType = currType->parent;
}

void basic(){
	if(*sym==INT){
		currType->typeCode = INT;
		sym++;
	}
	else if(*sym==BOOL){
		currType->typeCode = BOOL;
		sym++;
	}
	else error(BASIC);
}

void stmts(){
	if(*sym!=IF&&*sym!=WHILE&&*sym!=DO&&*sym!=BREAK&&*sym!=LSB&&*sym!=ID){
		return;
	}
	currSyntax->clear();
	stmt();
	stmts();
}

void stmt(){
	if(*sym==ID){
		string id = sym->value;
		isArrayInLeft = true; 
		loc();
		if(*sym==SET) sym++;
		else error(SET);
		currSyntax = currSyntax->derive();
		isArrayInLeft = false;
		bexpr();
		if(currSyntax->parent->offset==NULL){
			emit(opASSIGN, currSyntax->place, new Operand(), currSyntax->parent->place);
		}
		else{		//���鸳ֵ //һ��ֵ����һ������ 
			emit(opARRGETLEFT, currSyntax->place, currSyntax->parent->offset, currSyntax->parent->place);
		}
		currSyntax = currSyntax->parent;
		if(*sym==SC) sym++;
		else error(SC);
	}
	else if(*sym==IF){
		sym++;
		if(*sym==LP) sym++;
		else error(LP);
		bexpr();
		int quadNum1 = nextQuad(); 
		emit(opJZ, currSyntax->place, new Operand(), new Operand(-1, true));
		if(*sym==RP) sym++;
		else error(RP);
		stmt();
		int quadNum2 = nextQuad();
		emit(opJMP, new Operand(), new Operand(), new Operand(-1, true));
		int quadNum3 = nextQuad();
		elsmt();		// Ϊ��ʱ��������ת������������ ����ת����һ��
		backpatch(quadNum2, nextQuad());
		backpatch(quadNum1, quadNum3);
	}
	else if(*sym==WHILE){
		sym++;
		if(*sym==LP) sym++;
		else error(LP);
		int quadNum1 = nextQuad(); 
		bexpr();
		int quadNum2 = nextQuad(); 
		emit(opJZ, currSyntax->place, new Operand(), new Operand(-1, true));
		if(*sym==RP) sym++;
		else error(RP);
		currBreakQuad = currBreakQuad->derive();		//While֮������� 
		stmt();
		emit(opJMP, new Operand(), new Operand(), new Operand(quadNum1, true));		//ÿ��ѭ������ֵ������ 
		backpatch(quadNum2, nextQuad());
		if(currBreakQuad->breaklist.size()){		//break���ֻ����ǰ���ڵ�while������ת 
			backpatch(currBreakQuad->breaklist, nextQuad()); 
			currBreakQuad->breaklist.clear();
		}
		currBreakQuad = currBreakQuad->parent;
	}
	else if(*sym==DO){
		*sym++;
		int quadNum1 = nextQuad(); 
		currBreakQuad = currBreakQuad->derive();		//While֮������� 
		stmt();
		if(*sym==WHILE) *sym++;
		else error(WHILE);
		if(*sym==LP) *sym++;
		else error(LP);
		bexpr();
		if(*sym==RP) *sym++;
		else error(RP);
		if(*sym==SC) *sym++;
		else error(SC);
		emit(opJNZ, currSyntax->place, new Operand(), new Operand(quadNum1, true));	//Ϊ������� 
		if(currBreakQuad->breaklist.size()){		//�����������ʽ�� 
			backpatch(currBreakQuad->breaklist, nextQuad()); 
			currBreakQuad->breaklist.clear();
		}
		currBreakQuad = currBreakQuad->parent;
	}
	else if(*sym==BREAK){
		currBreakQuad->breaklist.push_back(nextQuad());
		emit(opJMP, new Operand(), new Operand(), new Operand(-1, true));
		sym++;
		if(*sym==SC) sym++;
		else error(SC);
	}
	else block();
}

void elsmt(){
	if(*sym!=ELSE){
		return;
	}
	sym++;
	stmt();
}

void loc(){
	string id;
//	checkingType->clear();	//��Ƕʱ����parent����ΪNULL  //��BUG 
	checkingType->elem = NULL;
	if(*sym==ID){
		id = sym->value;
		currSyntax->place = new Operand(topTable->getElem(id));
		sym++;
	}
	else error(ID);
	currSyntax = currSyntax->derive();
	currArray = topTable->getArrAddress(id);
	if(currArray!=NULL) currSyntax->i = new Operand(0);		//��Ҫ��������� 
	locp();
	if(!topTable->isLegal(id, checkingType)){
		hasIdError = true;
		exit(0);		//����ǿ�ƽ��� 
	}
	if(currArray!=NULL) currSyntax->parent->offset = currSyntax->s;
	else currSyntax->parent->place = currSyntax->s;
	if(currArray!=NULL&&isArrayInLeft==false){								//����Ϊ�������ڸ�ֵ���ʽ�ұ�ʱ,��arr2 
		Operand *new_operand = newTemp(currArray->typeCode);		// arr1[][] = arr2[][] , ��arr2[][]��ֵ��������ʱ����T
		emit(opARRGETRIGHT, currSyntax->parent->place, currSyntax->s, new_operand);
		currSyntax->parent->place = new_operand;
	}
	currSyntax = currSyntax->parent;
}

void locp(){
	if(*sym!=LB){
		currSyntax->s = currSyntax->i;
		return;
	}
	if(*sym==LB) sym++;		//����
	Type *tmp = currArray;  	//���Ϳ��ܻ�� �������ܲ�Ϊ���� 
	aexpr();
	currArray = tmp;
	//temp1 = locp.i * size    temp2 = temp1.place + aexpr.place
	Operand *new_operand1 = newTemp(currArray->typeCode);		// û�п��ǻ���������� 
	emit(opMUL, currSyntax->i, new Operand(currArray->size), new_operand1);
	Operand *new_operand2 = newTemp(currArray->typeCode);
	emit(opADD, new_operand1, currSyntax->place, new_operand2);
	if(*sym==RB) sym++;
	else error(RB);
	checkingType = checkingType->derive(0);		//ֻ���ά�� 
	currSyntax = currSyntax->derive();
	currSyntax->i = new_operand2;
	currArray = currArray->elem;
	locp();
	currSyntax->parent->s = currSyntax->s;
	currArray = currArray->parent;
	currSyntax = currSyntax->parent;
	checkingType = checkingType->parent;
}

void bexpr(){
	join();
	currSyntax = currSyntax->derive(); 
	bexprp();
	currSyntax->parent->place = currSyntax->s;
	currSyntax = currSyntax->parent;
}
void bexprp(){
	if(*sym!=OR){
		currSyntax->s = currSyntax->i;
		return;
	}
	if(*sym==OR) sym++;
	join();
//	emitNewTemp(opOR);
	Operand *new_operand = newTemp(checkingType->typeCode); 
	emit(opOR, currSyntax->i, currSyntax->place, new_operand);
	currSyntax = currSyntax->derive();
	currSyntax->i = new_operand;
	bexprp();
	currSyntax->parent->s = currSyntax->s;
	currSyntax = currSyntax->parent;
}

void join(){
	equality();
	currSyntax = currSyntax->derive(); 
	joinp();
	currSyntax->parent->place = currSyntax->s;
	currSyntax = currSyntax->parent;
}

void joinp(){
	if(*sym!=AND){
		currSyntax->s = currSyntax->i;
		return;
	}
	if(*sym==AND) sym++;
	equality();
//	emitNewTemp(opAND);
	Operand *new_operand = newTemp(checkingType->typeCode); 
	emit(opAND, currSyntax->i, currSyntax->place, new_operand);
	currSyntax = currSyntax->derive();
	currSyntax->i = new_operand;
	joinp();
	currSyntax->parent->s = currSyntax->s;
	currSyntax = currSyntax->parent;
}

void equality(){
	rel();
	currSyntax = currSyntax->derive(); 
	equalityp();
	currSyntax->parent->place = currSyntax->s;
	currSyntax = currSyntax->parent;
}

void equalityp(){
	if(*sym!=EQ&&*sym!=NE){
		currSyntax->s = currSyntax->i;
		return;
	}
	Operand *new_operand = newTemp(checkingType->typeCode);
	if(*sym==EQ){
		sym++;
		rel();
//		emitNewTemp(opEQ);
		emit(opJE, currSyntax->i, currSyntax->place, new Operand(nextQuad()+3, true));
		emit(opASSIGN, new Operand(0), new Operand(), new_operand);
		emit(opJMP, new Operand(), new Operand(), new Operand(nextQuad()+2, true));
		emit(opASSIGN, new Operand(1), new Operand(), new_operand);
		currSyntax = currSyntax->derive();
		currSyntax->i = new_operand;
		equalityp();
		currSyntax->parent->s = currSyntax->s;
		currSyntax = currSyntax->parent;
	}
	else if(*sym==NE){
		sym++;
		rel();
		emit(opJNE, currSyntax->i, currSyntax->place, new Operand(nextQuad()+3, true));
		emit(opASSIGN, new Operand(0), new Operand(), new_operand);
		emit(opJMP, new Operand(), new Operand(), new Operand(nextQuad()+2, true));
		emit(opASSIGN, new Operand(1), new Operand(), new_operand);
		currSyntax = currSyntax->derive();
		currSyntax->i = new_operand;
		equalityp();
		currSyntax->parent->s = currSyntax->s;
		currSyntax = currSyntax->parent;
	}
}

void rel(){
	aexpr();
	currSyntax = currSyntax->derive(); 
	relp();
	currSyntax->parent->place = currSyntax->s;
	currSyntax = currSyntax->parent;
}

void relp(){
	if(*sym!=LT&&*sym!=LE&&*sym!=GT&&*sym!=GE){
		currSyntax->s = currSyntax->i;
		return;
	}
	Operand *new_operand = newTemp(checkingType->typeCode);
	if(*sym==LT){
		sym++;
		aexpr();
		emit(opJL, currSyntax->i, currSyntax->place, new Operand(nextQuad()+3, true));
	}
	else if(*sym==LE){
		sym++;
		aexpr();
		emit(opJLE, currSyntax->i, currSyntax->place, new Operand(nextQuad()+3, true));
	}
	else if(*sym==GT){
		sym++;
		aexpr();
		emit(opJG, currSyntax->i, currSyntax->place, new Operand(nextQuad()+3, true));
	}
	else if(*sym==GE){
		sym++;
		aexpr();
		emit(opJGE, currSyntax->i, currSyntax->place, new Operand(nextQuad()+3, true));
	}
	emit(opASSIGN, new Operand(0), new Operand(), new_operand);
	emit(opJMP, new Operand(), new Operand(), new Operand(nextQuad()+2, true));
	emit(opASSIGN, new Operand(1), new Operand(), new_operand);
	currSyntax->s = new_operand;
}

void aexpr(){
	term();
	currSyntax = currSyntax->derive();
	aexprp();
	currSyntax->parent->place = currSyntax->s;
	currSyntax = currSyntax->parent;	
}

void aexprp(){
	if(*sym!=ADD&&*sym!=SUB){
		currSyntax->s = currSyntax->i;
		return;
	}
	if(*sym==ADD){
		sym++;
		term();
//		new_temp = newTemp(topTable, currType->typeCode);		// û�п��ǻ���������� 
//		new_operand = new Operand(new_temp);
//		emit(opADD, currSyntax->i, currSyntax->place, new_operand);
//		currSyntax = currSyntax->derive();
//		currSyntax->i = new_operand;
//		emitNewTemp(opADD);
		Operand *new_operand = newTemp(checkingType->typeCode); 
		emit(opADD, currSyntax->i, currSyntax->place, new_operand);
		currSyntax = currSyntax->derive();
		currSyntax->i = new_operand;
		aexprp();
		currSyntax->parent->s = currSyntax->s;
		currSyntax = currSyntax->parent;
	}
	else if(*sym==SUB){
		sym++;
		term();
//		emitNewTemp(opSUB);
		Operand *new_operand = newTemp(checkingType->typeCode); 
		emit(opSUB, currSyntax->i, currSyntax->place, new_operand);
		currSyntax = currSyntax->derive();
		currSyntax->i = new_operand;
		aexprp();
		currSyntax->parent->s = currSyntax->s;
		currSyntax = currSyntax->parent;
	}
}

void term(){
	unary();
	currSyntax = currSyntax->derive();
	termp();
	currSyntax->parent->place = currSyntax->s;
	currSyntax = currSyntax->parent;
}

void termp(){
	if(*sym!=MUL&&*sym!=DIV){
		currSyntax->s = currSyntax->i;
		return;
	}
	if(*sym==MUL){
		sym++;
		unary();
//		emitNewTemp(opMUL);
		Operand *new_operand = newTemp(checkingType->typeCode); 
		emit(opMUL, currSyntax->i, currSyntax->place, new_operand);
		currSyntax = currSyntax->derive();
		currSyntax->i = new_operand;
		termp();
		currSyntax->parent->s = currSyntax->s;
		currSyntax = currSyntax->parent;
	}
	else if(*sym==DIV){
		sym++;
		unary();
//		emitNewTemp(opDIV);
		Operand *new_operand = newTemp(checkingType->typeCode); 
		emit(opDIV, currSyntax->i, currSyntax->place, new_operand);
		currSyntax = currSyntax->derive();
		currSyntax->i = new_operand;
		termp();
		currSyntax->parent->s = currSyntax->s;
		currSyntax = currSyntax->parent;
	}
}

void unary(){
	Operand *new_operand;
	if(*sym==NOT){
		sym++;
		unary();	
		new_operand = newTemp(checkingType->typeCode);
		emit(opNOT, currSyntax->place, new Operand(), new_operand);
		currSyntax->place = new_operand;
	}
	else if(*sym==SUB){
		sym++;
		unary();
		new_operand = newTemp(checkingType->typeCode);
		emit(opSUB, currSyntax->place, new Operand(), new_operand);
		currSyntax->place = new_operand;
	}
	else{
		factor();
		// no operate
	}
}

void factor(){
	if(*sym==LP){
		if(*sym==LP) sym++;	//����д���ṹ��������ļ�д�� 	
		else error(LP);
		currSyntax = currSyntax->derive();
		bexpr();
		currSyntax->parent->place = currSyntax->place;
		currSyntax = currSyntax->parent;
		if(*sym==RP) sym++; 
		else error(RP);
	}
	else if(*sym==ID){
		currSyntax = currSyntax->derive();	//���Բ������� 
		loc(); 
		currSyntax->parent->place = currSyntax->place;
		currSyntax = currSyntax->parent;
	}
	else if(*sym==NUM){
		if(*sym==NUM){
			currSyntax->place = new Operand(atoi(sym->value.c_str()));
			sym++;
		}
		else error(NUM);
	}
	else if(*sym==FALSE){
		if(*sym==FALSE){
			currSyntax->place = new Operand(0);
			sym++;
		} 
		else error(FALSE);
	}
	else if(*sym==TRUE){
		if(*sym==TRUE){
			currSyntax->place = new Operand(1);
			sym++;
		}
		else error(TRUE);
	}
}

//�﷨������ 
void error(int expect){
	sym++;
	fprintf(stderr, "[-]SYNTAX ERROR: Expected %d\n", expect);
	exit(0); 		//ǿ���˳������⴦�����ķ��Ĵ���ʱ���������׳�Ի�����
}

//bool isBoolOperator(WordUnit word){
//	switch(word.code){
//		case EQ: 
//		case NE:
//		case AND:
//		case OR:
//		case NOT:
//		case LE:
//		case GE:
//		case LT:
//		case GT:
//			return true;
//		default:
//			return false;
//	}
//}


