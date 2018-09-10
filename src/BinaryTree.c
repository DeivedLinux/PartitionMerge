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
	struct TreeNode* father;
	struct TreeNode* leftSon;
	struct TreeNode* rightSon;
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

static void PreOrder(Root root);


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


static TreeNode newNodeCreate(void)
{
	TreeNode node = NULL;

	try
	{
		node = (TreeNode)malloc(sizeof(struct TreeNode));
		if(node != NULL)
		{
			node->father = NULL;
			node->leftSon = NULL;
			node->rightSon = NULL;
			node->file = NULL;
			node->winner = 0;
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

	return node;
} 

unsigned buildTreeWinners(BinaryTreeWinners tree, ArrayList listFiles, FunctionReadFile readFile)
{
	TreeNode *leavesVect;
	TreeNode newNode;
	FILE* partition;
	Node auxiliary;
	unsigned k,i;
	unsigned nFiles;
	unsigned nodes = 0;

	nFiles = getListLimit(listFiles);
	leavesVect = (TreeNode*)calloc(nFiles, sizeof(TreeNode));


	try
	{
		if(tree != NULL && TreeRoot(tree) != NULL)
		{
			foreach_ArrayList(partition, listFiles)
			{
				
				k = readFile(partition);
				newNode = newNodeCreate();
				newNode->file = partition;
				newNode->winner = k;
				printf("%i ", k);
				leavesVect[nodes] = newNode;
				nodes = nodes + 1;
				

			}
			nodes = 0;
		
			while(nFiles > 1)
			{
				printf("nFiles: %i\n", nFiles);
				for(i = 0; i < nFiles-1; i += 2)
				{
					newNode = newNodeCreate();
					if(leavesVect[i]->winner < leavesVect[i + 1]->winner)
					{
						newNode->file = leavesVect[i]->file;
						newNode->winner = leavesVect[i]->winner;
						newNode->leftSon = leavesVect[i];
						newNode->rightSon = leavesVect[i + 1];
					}
					else
					{
						newNode->file = leavesVect[i + 1]->file;
						newNode->winner = leavesVect[i + 1]->winner;
						newNode->leftSon = leavesVect[i];
						newNode->rightSon = leavesVect[i + 1];	
					}

					leavesVect[nodes] = newNode;
					nodes += 1;
				}

				if((i-nFiles) > 0)
				{
					leavesVect[nodes] = leavesVect[nFiles-1]; 
				}
				nodes = 0;
				nFiles = ((nFiles/2) + (nFiles%2));
				if(nFiles == 1)
				{	
					*TreeRoot(tree) = leavesVect[0];
				}
	
			}
			
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

static void PreOrder(Root root)
{
	if(*root != NULL)
	{
		printf("%i\n",(*root)->winner);
		PreOrdem(&((*root)->leftSon));
		PreOrdem(&((*root)->rightSon));
	}
}

void BinaryTreePreOrder(BinaryTreeWinners tree)
{
	PreOrdem(TreeRoot(tree));
}



