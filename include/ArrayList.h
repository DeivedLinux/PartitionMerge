/*
	Author: Deived William da silva Azevedo
	Date: 01/09/2018
	File: ArrayList.h
	Dependencies: stdlib.h, stdbool.h
*/


#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <stdlib.h>
#include <stdbool.h>
#include "Typedefs.h"


#define CastingUnPtr(List) ((Node)(*(*((unsigned long long***)List))))

#define foreach_ArrayList(Obj, List)  \
	for(Node scroll = CastingUnPtr(List); scroll != NULL, Obj = scroll != NULL?scroll->data:0; scroll = scroll->next)



typedef struct Node
{
	struct Node* next;
	struct Node* previous;
	void* data;
	int index;
}*Node;

typedef int(*FunctionCompare)(Object, Object);
typedef void* ArrayList;
typedef void(*PrintFunctionFormat)(Object);

ArrayList newArrayList(int size);
bool isEmpty(ArrayList list);
Object removeTopList(ArrayList list);
Object removeBottomList(ArrayList list);
Object removeElementList(ArrayList list, Object data, FunctionCompare compare);
void insertTopList(ArrayList list, Object data);
void insertBottomList(ArrayList list, Object data);
void insertSorted(ArrayList list, Object data, FunctionCompare compare);
unsigned getListSize(ArrayList list);
void printList(ArrayList list, PrintFunctionFormat format);
void destroyList(ArrayList list);
void destroyArrayList(ArrayList list);
bool fullArrayList(ArrayList list);
unsigned getListLimit(ArrayList list);

Object* listToVector(ArrayList list);

#endif