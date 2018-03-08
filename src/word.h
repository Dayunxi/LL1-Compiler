#ifndef __LAB8_WORD__
#define __LAB8_WORD__
#include<string>
//词法分析结构体 
struct WordUnit{
	int code;
	std::string value;
	WordUnit();
	bool operator == (const int &code_) const;
	bool operator != (const int &code_) const;
}; 
//词法分析函数 
bool isDigital(char ch);
bool isLetter(char ch);
bool isSingleDelimiterOperator(char ch);
int reserve(std::string str);
void preProcess(char *src, char *dest, int len);
WordUnit lexical(char* &ch);
#endif
