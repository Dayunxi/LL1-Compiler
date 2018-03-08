#include<stdio.h>
#include"src/global.h"
#include"src/table.h"

using std::string;
using std::list;
using std::vector;

Type::Type(){
	size = 0;
	parent = elem = NULL;
}
void Type::clear(){
	size = 0;
	parent = elem = NULL;
}
int Type::space(){
	if(elem==NULL){
		if(typeCode==INT) return 4;
		else if(typeCode==BOOL) return 1;
		else return 0;
	}
	return size * elem->space();
}
int Type::dim() const{
	if(elem) return 1+elem->dim();
	else return 0;
}
Type* Type::derive(int size_){		// ÅÉÉú ¼Ì³Ð 
	Type *newType = new Type();
	newType->parent = this;
	newType->typeCode = this->typeCode;
	this->elem = newType;	
	this->size = size_;
	return newType;
}

TableElem::TableElem(string id_, Type tp_, int space_){
	id = id_;
	tp = tp_;
	space = space_;
}

Table::Table(){
	parent = child = sibling = NULL;
}
TableElem* Table::add(string id_, Type tp_){
	elems.push_back(TableElem(id_, tp_, tp_.space()));
	return &elems.back();
}
void Table::addChild(Table *t_){
	if(child==NULL) child = t_;
	else{
		Table *tmp;
		for(tmp=child; tmp->sibling!=NULL; tmp=tmp->sibling);
		tmp->sibling = t_;
	}		
}
void Table::traverse(int deep){
	for(Table *tmp=this; tmp!=NULL; tmp=tmp->sibling){
		for(list<TableElem>::iterator iter=tmp->elems.begin(); iter!=tmp->elems.end(); iter++){
			for(int i=0; i<deep; i++) printf("...");
			printf("ID: %s    Space: %d\n", iter->id.c_str(), iter->space);
		}
		tmp->child->traverse(deep+1);
	}		
}
Type* Table::search(string id){
	for(list<TableElem>::iterator iter=elems.begin(); iter!=elems.end(); iter++){
		if(iter->id==id) return &(iter->tp);
	}
	if(parent) return parent->search(id);
	else return NULL;
}
TableElem* Table::getElem(string id){
	for(list<TableElem>::iterator iter=elems.begin(); iter!=elems.end(); iter++){
		if(iter->id==id) return &(*iter);
	}
	if(parent) return parent->getElem(id);
	else return NULL;
}
bool Table::isLegal(string id, Type *tp_){
	const Type *tmp = search(id);
	if(!tmp){
		printf("Identifier not declared: %s\n", id.c_str());
		return false;
	}
	else if(tmp->dim()!=tp_->dim()){
		printf("Dimension of identifier not matched: %s\n", id.c_str());
		return false;
	}
	else return true;
}
Type* Table::getArrAddress(std::string id){
	Type *tmp = search(id);
	if(tmp!=NULL&&tmp->dim()!=0){
		return tmp;
	}
	else return NULL;
}
	
	
	
	
