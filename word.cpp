#include<stdio.h>
#include"src/global.h"
#include"src/word.h"

using std::string;

WordUnit::WordUnit(){
	code = 0;
	value = "";
}
bool WordUnit::operator == (const int &code_) const{
	return this->code == code_;
}
bool WordUnit::operator != (const int &code_) const{
	return this->code != code_;
}

//�ʷ����� 
WordUnit lexical(char* &ch){
	WordUnit word;
	string str_token = "";
	while(*ch == ' '||*ch == '\t'||*ch == '\n'||*ch == '\r') ch++; 		//�����հ��ַ� 
	if(isLetter(*ch)){		//��ʶ�� 
		while(isLetter(*ch) || isDigital(*ch)){
			str_token += *ch++;
		}
		int code = reserve(str_token);
		if(code){
			word.code = code;
		}
		else{
			word.code = ID;
			word.value = str_token;
		}
	}
	else if(isDigital(*ch)){	//���� 
		while(isDigital(*ch)){
			str_token += *ch++;
		}
		if(isLetter(*ch)){			//���ֺ���������ĸ���������� 
			word.code = -1;
			word.value = "Identifier Error";
		}
		else{
			word.code = NUM;
			word.value = str_token;
		}
	}
	else if(*ch == '='||*ch == '>'||*ch == '<'||*ch == '!'){	//>=, <=, ==, !=
		str_token += *ch++;
		if(*ch == '='){
			str_token += *ch++;
		}
		word.code = reserve(str_token);
	}
	else if(*ch == '|'||*ch == '&'){	//&&, ||
		str_token += *ch++;
		if(*ch == *(ch-1)){
			str_token += *ch++;
			word.code = reserve(str_token);
		}
		else{						//�Ƿ���� 
			word.code = -1;
			word.value = "Invaild Operator: ";
			word.value += *(ch-1);
		}
	}
	else if(isSingleDelimiterOperator(*ch)){	//��������� 
		str_token += *ch++;
		word.code = reserve(str_token);
	}
	else if(*ch){				//�Ƿ��ַ� 
		word.code = -1;
		word.value = "Invaild Symbol: ";
		word.value += *ch++;
	}
	//����Ϊ������'\0'
	return word;
}

bool isDigital(char ch){
	return ch >= '0' && ch <= '9';
}

bool isLetter(char ch){
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool isSingleDelimiterOperator(char ch){
	switch(ch){
		case '>': 
		case '<':
		case '=':
		case '+':
		case '-':
		case '*':
		case '/':
		case '|':
		case '&':
		case '!':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{': 
		case '}':
		case ';':
			return true;
		default:
			return false;
	}
}

int reserve(string str){
	if(str == "if") return IF;
	if(str == "else") return ELSE;
	if(str == "do") return DO;
	if(str == "while") return WHILE;
	if(str == "break") return BREAK;
	if(str == "int") return INT;
	if(str == "bool") return BOOL;
	if(str == "true") return TRUE;
	if(str == "false") return FALSE;
	if(str == "=") return SET;
	if(str == "!") return NOT;
	if(str == "+") return ADD;
	if(str == "-") return SUB;
	if(str == "*") return MUL;
	if(str == "/") return DIV;
	if(str == ">") return GT;
	if(str == "<") return LT;
	if(str == ">=") return GE;
	if(str == "<=") return LE;
	if(str == "!=") return NE;
	if(str == "==") return EQ;
	if(str == "&&") return AND;
	if(str == "||") return OR;
	if(str == ";") return SC;
	if(str == "(") return LP;
	if(str == ")") return RP;
	if(str == "{") return LSB;
	if(str == "}") return RSB;
	if(str == "[") return LB;
	if(str == "]") return RB;
	return 0;
}

//ȥע��  û��ȥ�հ׷� 
void preProcess(char *src, char *dest, int len){
	int srcIndex = 0, destIndex = 0;
	bool isCommit = false;
	while(srcIndex<len-1){
		if(src[srcIndex]=='/'&&src[srcIndex+1]=='/') isCommit = true;
		if(isCommit&&src[srcIndex]=='\n') isCommit = false;
		if(!isCommit) dest[destIndex++] = src[srcIndex];
		srcIndex++;
	}
	if(!isCommit) dest[destIndex] = src[srcIndex];	//���һ���ַ� 
}
