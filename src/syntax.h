#ifndef __LAB8_SYNTAX__
#define __LAB8_SYNTAX__
#include"word.h"
#include"table.h"
#include"semantic.h"
#include<vector>

//�﷨�������� 
void program(), block(), decls(), stmts(), elsmt(), decl(), type(), basic(), typep(), stmt(),
	loc(), locp(), bexpr(), bexprp(), join(), joinp(), equality(), equalityp(), rel(),
	relp(), aexpr(), aexprp(), term(), termp(), unary(), factor();
bool isBoolOperator(WordUnit word);
void error(int expect);
void output(WordUnit word);
void output(int syntax);

//�﷨�����ĵ�����ָ�� 
extern std::vector<WordUnit>::iterator sym;
//���ű���� 
extern Type *checkingType;
extern Type *currArray;
extern bool hasIdError;
extern bool isArrayInLeft;
extern bool isBoolControl;

#endif
