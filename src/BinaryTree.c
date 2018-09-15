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
	int addressWinner;
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
			node->addressWinner = None;
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
	unsigned k,i;
	unsigned nFiles;
	unsigned nodes = 0;
	ArrayList leavesList;
	FILE* outputFile = NULL;
	FILE* selectPartition;
	FILE* partition;
	FILE** filesVect;
	int res;

	nFiles = getListLimit(listFiles);
	leavesVect = (TreeNode*)calloc(nFiles, sizeof(TreeNode));
	filesVect = (FILE**)calloc(nFiles, sizeof(FILE*));

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
				newNode->addressWinner = nodes;
				filesVect[nodes] = partition;
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
			free(leavesVect);

			leavesVect = (TreeNode*)listToVector(leavesList);

			while((*TreeRoot(tree))->winner != HIGH_VALUE)
			{
				selectPartition = filesVect[(*TreeRoot(tree))->addressWinner];
			 	writeFile(selectPartition, outputFile);
			 	leavesVect[(*TreeRoot(tree))->addressWinner]->winner = readFile(selectPartition);
			 	PostOrder(TreeRoot(tree));
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
	free(filesVect);
	

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


