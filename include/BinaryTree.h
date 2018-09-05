/*
	Author: Deived William da silva Azevedo
	Date: 04/09/2018
	File: BinaryTree
	Dependencies: stdlib.h, stdbool.h
*/


#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include "ArrayList.h"


typedef struct Node
{
	struct Node* father;
	struct Node* leftSon;
	struct Node* rightSon;
	void* info;
}*Node;

typedef void* BinaryTree;

BinaryTree newBinaryTree(int size);
ArrayList getNodes(BinaryTree tree);

#endif