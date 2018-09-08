#include "../include/BinaryTree.h"
#include "../include/Typedefs.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/ArrayList.h"
#include "../include/Exception.h"

#define TreeRoot(Tree)   (((BTree)(Tree))->root)
#define TreeLeaves(Tree) (((BTree)(Tree))->limit)

typedef struct TreeNode
{
	struct Node* father;
	struct Node* leftSon;
	struct Node* rightSon;
	FILE* file;
	unsigned winner;
}*TreeNode;

typedef TreeNode* Root;

typedef enum
{
	LIMITED,
	UNLIMITED
}ListProperty;

typedef struct BTree
{
	Root root;
	unsigned size;
	unsigned limit;
	ListProperty property;
}*BTree;


BinaryTreeWinners newBinaryTreeWinners(int treeLeaves)
{
	BTree newTree = NULL;

	try
	{
		newTree = malloc(sizeof(struct BTree));
		if(newTree != NULL)
		{
			newTree->root = malloc(sizeof(Root));
			if(newTree->root != NULL)
			{
				*newTree->root = NULL;
				if(treeLeaves < 0)
				{
					newTree->property = UNLIMITED;
					newTree->size = 0;
				}
				else
				{
					newTree->property = LIMITED;
					newTree->limit = treeLeaves;
					newTree->size = 0;
				}
			}
			else
			{
				throw(__MemoryAllocationException__);
			}
		}
		else
		{
			throw(__MemoryAllocationException__);
		}
	}
	catch(MemoryAllocationException)
	{
		PrintExceptionStdOut(MemoryAllocationException);
	}

	return newTree;
}

unsigned buildTreeWinners(BinaryTreeWinners tree, ArrayList listFiles, FunctionReadFile readFile)
{
	TreeNode newNode;

	try
	{
		if(tree != NULL && TreeRoot(tree) != NULL)
		{
			
		}
		else
		{
			throw(__NullPointerException__);
		}
	}
	catch(NullPointerException)
	{
		PrintExceptionStdOut(NullPointerException);
	}
}

