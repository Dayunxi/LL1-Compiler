#ifndef __LAB8_TABLE__
#define __LAB8_TABLE__
#include<string>
#include<vector>
#include<list>
//符号表结构体 
struct Type{
	int typeCode;
	int size;		//元素个数  //数组也能为0 int[0][0] arr; 
	Type *elem, *parent;
	Type();
	void clear();
	int space();
	int dim() const;	//非零即数组 
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
	std::list<TableElem> elems;		//vector容器的元素地址会变 
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
