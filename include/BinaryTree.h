/*
	Author: Deived William da silva Azevedo
	Date: 04/09/2018
	File: BinaryTree
	Dependencies: stdlib.h, stdbool.h
*/


#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include "ArrayList.h"
#include <stdio.h>

typedef void* BinaryTreeWinners;
typedef unsigned(*FunctionReadFile)(FILE* file);


BinaryTreeWinners newBinaryTreeWinners(int treeLeaves);
unsigned buildTreeWinners(BinaryTreeWinners tree, ArrayList listFiles, FunctionReadFile readFile);
void MountBTreeWinners(BinaryTreeWinners tree);
void BinaryTreePreOrdem(BinaryTreeWinners tree);

#endif