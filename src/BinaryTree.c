#include "BinaryTree.h"
#include "Typedefs.h"
#include <stdio.h>
#include <stdlib.h>
#include "ArrayList.h"
#include "Exception.h"

typedef Node* Root;
typedef void(*Method)(BinaryTree, Object);

typedef enum
{
	LIMITED,
	UNLIMITED
}ListProperty;

typedef struct BTree
{
	Root tree;
	unsigned size;
	unsigned limit;
	ListProperty property;
}*BTree;


BinaryTree newBinaryTree(int size)
{
	BTree newTree = NULL;

	try
	{
		newTree = malloc(sizeof(struct BTree));
		if(newTree != NULL)
		{

		}
		else
		{
			throw(__MemoryAllocationException__);
		}
	}
	catch(MemoryAllocationException)
	{

	}

	return newTree;
}