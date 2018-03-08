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
	while(scanf("%c", &temp[i++]) != EOF); //�����ո񼰻��з�, ����EOF
	preProcess(temp, buffer, i);
//	printf("%s\n", buffer);
	fprintf(stderr, "\n");
	while(*p_curr){				//�ʷ����� 
		word = lexical(p_curr);
		words.push_back(word);
		if(word.code == -1){
			fprintf(stderr, "[-]WORD ERROR: %s\n", word.value.c_str());
			hasWordError = true;
		}
	}
	if(hasWordError) return 0;	//�ʷ����� 
	
	sym = words.begin();
	program();		//�﷨���������ű������������ 
	
	if(hasIdError) return 0;	//��������ʹ�� 
	//freopen("quadList.txt", "wt", stdout);		//����ض��� 
	i = 0;
	for(vector<QuadTuple>::iterator iter=sequence.begin(); iter!=sequence.end(); iter++){
		fprintf(stderr, "%d ", i++);		//������ 
		printQuad(*iter);
		fprintf(stdout, "\n");
	}
	//fprintf(stderr, "\nPrint to quadList.txt\n");
	return 0;
} 





