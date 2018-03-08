#include"src/common.h"
#include"src/word.h"
#include"src/table.h"
#include<string>
/**
这些变量都是语法分析和语义分析同时要用到的 
**/ 
using std::vector;

//符号表变量 
Table *topTable = NULL;
Type *currType = new Type();		//通过该变量在不同层的语法分析函数间传递值 

