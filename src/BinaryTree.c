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
static void PostOrder(Root root, FunctionReadFile readFile);


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

FILE* InterweaveTree(BinaryTreeWinners tree, ArrayList listFiles, FunctionReadFile readFile, FunctionWriteFile writeFile, const int HIGH_VALUE)
{
	TreeNode *leavesVect;
	TreeNode newNode;
	FILE* partition;
	Node auxiliary;
	unsigned k,i;
	unsigned nFiles;
	unsigned nodes = 0;
	ArrayList leavesList;
	FILE* outputFile = NULL;
	FILE* selectPartition;
	int res;

	nFiles = getListLimit(listFiles);
	leavesVect = (TreeNode*)calloc(nFiles, sizeof(TreeNode));
	leavesList = newArrayList(nFiles);
	FileOpen(outputFile, "OutputFile/output.bin", "w+b");


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
				leavesVect[nodes] = newNode;
				insertBottomList(leavesList, newNode);
				nodes = nodes + 1;
			}
			nodes = 0;
		
			while(nFiles > 1)
			{
				for(i = 0; i < nFiles-1; i += 2)
				{
					newNode = newNodeCreate();
					if(leavesVect[i]->winner < leavesVect[i + 1]->winner)
					{
						newNode->file = leavesVect[i]->file;
						newNode->winner = leavesVect[i]->winner;
						newNode->leftSon = leavesVect[i];
						newNode->rightSon = leavesVect[i + 1];
						leavesVect[i]->father = newNode;
						leavesVect[i + 1]->father = newNode;
					}
					else
					{
						newNode->file = leavesVect[i + 1]->file;
						newNode->winner = leavesVect[i + 1]->winner;
						newNode->leftSon = leavesVect[i];
						newNode->rightSon = leavesVect[i + 1];
						leavesVect[i]->father = newNode;
						leavesVect[i + 1]->father = newNode;	
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
			free(leavesVect);

			selectPartition = (*TreeRoot(tree))->file;
			ReWind(selectPartition);
			writeFile(selectPartition, outputFile, 0);

			leavesVect = (TreeNode*)listToVector(leavesList);
			PostOrder(TreeRoot(tree), readFile);
			printf("%i\n",(*TreeRoot(tree))->winner);
			//FileWrite()

			
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

	

	return outputFile;
}

static void PreOrder(Root root)
{
	if(*root != NULL)
	{
		printf("%i\n",(*root)->winner);
		PreOrder(&((*root)->leftSon));
		PreOrder(&((*root)->rightSon));
	}
}

static void PostOrder(Root root, FunctionReadFile readFile)
{
	if(*root != NULL)
	{
		PostOrder(&((*root)->leftSon), readFile);
		PostOrder(&((*root)->rightSon), readFile);
		if((*root)->leftSon == NULL && (*root)->rightSon == NULL)
		{
			(*root)->winner = readFile((*root)->file);
		}
		else if((*root)->leftSon->winner < (*root)->rightSon->winner)
		{
			(*root)->winner = (*root)->leftSon->winner;
		}
		else
		{
			(*root)->winner = (*root)->rightSon->winner;	
		}

	}
}

void BinaryTreePreOrder(BinaryTreeWinners tree)
{
	PreOrder(TreeRoot(tree));
}

void BinaryTreePostOrder(BinaryTreeWinners tree)
{
	//PostOrder(TreeRoot(tree));
}


