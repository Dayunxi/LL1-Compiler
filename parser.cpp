#include<cstdio>
#include<vector>
#include"src/global.h"
#include"src/word.h"
#include"src/syntax.h"
#include"src/semantic.h"
#include"src/common.h"

#define MAXLEN 20000

using namespace std;

int main(){
	char *p_curr, buffer[MAXLEN], temp[MAXLEN];
	int i = 0;
	bool hasWordError = false; 
	WordUnit word;
	vector<WordUnit> words;
	p_curr = buffer;
	while(scanf("%c", &temp[i++]) != EOF); //包含空格及换行符, 读至EOF
	preProcess(temp, buffer, i);
//	printf("%s\n", buffer);
	fprintf(stderr, "\n");
	while(*p_curr){				//词法分析 
		word = lexical(p_curr);
		words.push_back(word);
		if(word.code == -1){
			fprintf(stderr, "[-]WORD ERROR: %s\n", word.value.c_str());
			hasWordError = true;
		}
	}
	if(hasWordError) return 0;	//词法错误 
	
	sym = words.begin();
	program();		//语法分析及符号表建立及语义分析 
	
	if(hasIdError) return 0;	//变量错误使用 
	//freopen("quadList.txt", "wt", stdout);		//输出重定向 
	i = 0;
	for(vector<QuadTuple>::iterator iter=sequence.begin(); iter!=sequence.end(); iter++){
		fprintf(stderr, "%d ", i++);		//输出序号 
		printQuad(*iter);
		fprintf(stdout, "\n");
	}
	//fprintf(stderr, "\nPrint to quadList.txt\n");
	return 0;
} 





