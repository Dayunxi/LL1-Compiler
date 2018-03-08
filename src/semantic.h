#ifndef __LAB8_SEMANTIC__
#define __LAB8_SEMANTIC__
#include"table.h"
#include"syntax.h" 
#include<list>
#include<vector>
// 操作数
struct Operand{
	int type;  // 0=none  1=immediate  2=variable	3=address
	int immediate;
	TableElem *variable;
	int address;
	Operand();
	Operand(TableElem *temp);		//产生一个变量操作数 
	Operand(int n);			//产生一个立即数 
	Operand(int addr, bool isAddress);		//一个四元式地址 
}; 

// 四元式
struct QuadTuple{
	int op;
	Operand operand1, operand2;
	Operand result;
	QuadTuple();
	QuadTuple(int op_, Operand op1, Operand op2, Operand res);
}; 

//语义结构体 
struct Syntax{
	Operand *i, *s;		//属性，指向一个操作数 
	Operand *place, *offset;	//offset用于数组元素的引用 
	Syntax *parent;
	Syntax();
	void clear();
	Syntax* derive();
};
//控制语句及布尔表达式的翻译 
//struct BoolControl{
//	std::list<int> truelist, falselist;
//	std::list<int> i_truelist, i_falselist;
//	BoolControl *parent;
//	BoolControl();
//	BoolControl* derive();
//};
//struct ControlFlow{
//	std::list<int> i;
//	std::list<int> nextlist;
//	ControlFlow *parent;
//	ControlFlow();
//	ControlFlow* derive();
//};
struct BreakList{
	std::list<int> i;
	std::list<int> breaklist;
	BreakList *parent;
	BreakList();
	BreakList* derive();
};

//语义分析函数
QuadTuple* emit(int op, Operand *operand1, Operand *operand2, Operand *result); 
Operand* newTemp(int typecode);
void printQuad(QuadTuple tuple);
void emitNewTemp(int opType);
std::list<int> merge(std::list<int> list1, std::list<int> list2);
void backpatch(std::list<int> jmplist, int quadIndex);
void backpatch(int quad, int quadIndex);
int nextQuad();

extern Syntax *currSyntax;
//extern BoolControl *currBoolean;
//extern ControlFlow *currControl;
extern BreakList *currBreakQuad;
extern std::vector<QuadTuple> sequence;
extern int currTempNum;
#endif 
