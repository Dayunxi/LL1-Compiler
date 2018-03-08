#ifndef __LAB8_WORD__
#define __LAB8_WORD__
#include<string>
//�ʷ������ṹ�� 
struct WordUnit{
	int code;
	std::string value;
	WordUnit();
	bool operator == (const int &code_) const;
	bool operator != (const int &code_) const;
}; 
//�ʷ��������� 
bool isDigital(char ch);
bool isLetter(char ch);
bool isSingleDelimiterOperator(char ch);
int reserve(std::string str);
void preProcess(char *src, char *dest, int len);
WordUnit lexical(char* &ch);
#endif
