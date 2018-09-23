#include "../include/BinaryTree.h"
#include "../include/Typedefs.h"
#include "../include/ArrayList.h"
#include "../include/Exception.h"
#include <stdio.h>
#include <stdlib.h>

#define TreeRoot(Tree)   (((BTree)(Tree))->root)
#define TreeLeaves(Tree) (((BTree)(Tree))->limit)
#define None -1

typedef struct TreeNode
{
	struct TreeNode* father;
	struct TreeNode* leftSon;
	struct TreeNode* rightSon;
	FILE* addressWinner;
	unsigned winner;
}*TreeNode;

typedef TreeNode* Root;


typedef struct BTree
{
	Root root;
	unsigned size;
}*BTree;

static void PreOrder(Root root);
static void PostOrder(Root root);


BinaryTreeWinners newBinaryTreeWinners(void)
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
				newTree->size = 0;
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
			node->addressWinner = NULL;
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

static void UpdateWinnerTreerRoot(Root root, FunctionReadFile readFile)
{
	if((*root)->leftSon != NULL && (*root)->rightSon != NULL)
	{
		if((*root)->leftSon->winner < (*root)->rightSon->winner)
		{
			UpdateWinnerTreerRoot(&((*root)->leftSon), readFile);
		}
		else
		{
			UpdateWinnerTreerRoot(&((*root)->rightSon), readFile);
		}

		if((*root)->leftSon->winner < (*root)->rightSon->winner)
		{
			(*root)->winner = (*root)->leftSon->winner;
			(*root)->addressWinner = (*root)->leftSon->addressWinner;
		}
		else
		{
			(*root)->winner = (*root)->rightSon->winner;
			(*root)->addressWinner = (*root)->rightSon->addressWinner;
		}
	}
	else
	{
		(*root)->winner = readFile((*root)->addressWinner);
	}
}

FILE* InterweaveTree(const OUTPUT outmode, BinaryTreeWinners tree, ArrayList listFiles, FunctionReadFile readFile, FunctionWriteFile writeFile, const int HIGH_VALUE)
{
	TreeNode *leavesVect;
	TreeNode newNode;
	unsigned k,i;
	unsigned nFiles;
	unsigned nodes = 0;
	ArrayList leavesList;
	FILE* outputFile = NULL;
	FILE* selectPartition;
	FILE* partition;
	int res;


	try
	{
		if(tree != NULL && TreeRoot(tree) != NULL)
		{
			try
			{
				nFiles = getListSize(listFiles);
				if(nFiles == 1)
				{
					throw(__TreeWinnerException__);
				}
			}
			catch(TreeWinnerException)
			{
				PrintExceptionStdOut(TreeWinnerException);
			}


			leavesVect = (TreeNode*)calloc(nFiles, sizeof(TreeNode));

			leavesList = newArrayList(nFiles);

			if(outmode == OUT_FILESTD)
			{
				FileOpen(outputFile, "OutputFile/output.bin", "w+b");
			}
			else
			{
				static unsigned temp;
				char bufferStr[64];

				sprintf(bufferStr,"TempFiles/Temp %u.bin", temp);

				FileOpen(outputFile, bufferStr, "w+b");
				temp += 1;
			}

			foreach_ArrayList(partition, listFiles)
			{
				
				k = readFile(partition);
				newNode = newNodeCreate();
				newNode->addressWinner = partition;
				newNode->winner = k;
				leavesVect[nodes] = newNode;
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
						newNode->addressWinner = leavesVect[i]->addressWinner;
						newNode->winner = leavesVect[i]->winner;
						newNode->leftSon = leavesVect[i];
						newNode->rightSon = leavesVect[i + 1];
						leavesVect[i]->father = newNode;
						leavesVect[i + 1]->father = newNode;
					}
					else
					{
						newNode->addressWinner = leavesVect[i + 1]->addressWinner;
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
					(*TreeRoot(tree))->father = NULL;
				}
			}

			while((*TreeRoot(tree))->winner != HIGH_VALUE)
			{
				selectPartition = (*TreeRoot(tree))->addressWinner;
			 	writeFile(selectPartition, outputFile);
			 	UpdateWinnerTreerRoot(TreeRoot(tree), readFile);
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

	foreach_ArrayList(partition, listFiles)
	{
		FileClose(partition);
	}
	destroyArrayList(leavesList);

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


static void PostOrder(Root root)
{
	if(*root != NULL)
	{
		PostOrder(&((*root)->leftSon));
		PostOrder(&((*root)->rightSon));

		if((*root)->leftSon != NULL && (*root)->rightSon != NULL)
		{
			if((*root)->leftSon->winner < (*root)->rightSon->winner)
			{
				(*root)->winner = (*root)->leftSon->winner;
				(*root)->addressWinner = (*root)->leftSon->addressWinner;
			}
			else
			{
				(*root)->winner = (*root)->rightSon->winner;
				(*root)->addressWinner = (*root)->rightSon->addressWinner;
			}
		}
	}
}

void BinaryTreePreOrder(BinaryTreeWinners tree)
{
	PreOrder(TreeRoot(tree));
}


void BinaryTreePostOrder(BinaryTreeWinners tree)
{
	PostOrder(TreeRoot(tree));
}


