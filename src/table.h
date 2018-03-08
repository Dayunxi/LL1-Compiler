#ifndef __LAB8_TABLE__
#define __LAB8_TABLE__
#include<string>
#include<vector>
#include<list>
//���ű�ṹ�� 
struct Type{
	int typeCode;
	int size;		//Ԫ�ظ���  //����Ҳ��Ϊ0 int[0][0] arr; 
	Type *elem, *parent;
	Type();
	void clear();
	int space();
	int dim() const;	//���㼴���� 
	Type* derive(int size_);
};

struct TableElem{
	std::string id;
	Type tp;
	int space;
	TableElem(std::string id_, Type tp_, int space_);
};

struct Table{		//binary tree
	Table *parent, *child, *sibling;
	std::list<TableElem> elems;		//vector������Ԫ�ص�ַ��� 
	Table();
	TableElem *add(std::string id_, Type tp_);
	void addChild(Table *t_);
	void traverse(int deep);
	Type* search(std::string id);
	TableElem* getElem(std::string id);
	bool isLegal(std::string id, Type *tp_);
	Type* getArrAddress(std::string id);
};


#endif
