/*
	Author: Deived William da silva Azevedo
	Date: 04/09/2018
	File: BinaryTree
	Dependencies: stdlib.h, stdbool.h, ArrayList.h
*/


#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include "ArrayList.h"
#include <stdio.h>

typedef void* BinaryTreeWinners;
typedef unsigned(*FunctionReadFile)(FILE* file);
typedef void(*FunctionWriteFile)(FILE* partition, FILE* outputFile);

typedef enum
{
	OUT_FILETEMP,
	OUT_FILESTD
}OUTPUT;

BinaryTreeWinners newBinaryTreeWinners(void);
FILE* InterweaveTree(const OUTPUT outmode, BinaryTreeWinners tree, ArrayList listFiles, FunctionReadFile readFile, FunctionWriteFile writeFile, const int HIGH_VALUE);
void BinaryTreePreOrder(BinaryTreeWinners tree);
void BinaryTreePostOrder(BinaryTreeWinners tree);



#endif
