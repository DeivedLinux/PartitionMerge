#include "../include/ArrayList.h"
#include "../include/Exception.h"
#include "../include/Typedefs.h"
#include <stdio.h>
#include <stdlib.h>

#define CList(Obj) 			((List)Obj)
#define GetListHead(Obj)	(((List)Obj)->ptrlistHead)
#define GetListEnd(Obj)		(((List)Obj)->ptrListEnd)


typedef Node* ListHead;

typedef enum
{
	LIMITED,
	UNLIMITED
}ListProperty;

typedef struct List
{
	ListHead ptrlistHead;
	ListHead ptrListEnd;
	ListProperty property;
	unsigned size; 
	unsigned limit;
}*List;

static bool checkListLimit(ArrayList list)
{
	return ((CList(list)->property == LIMITED) && (CList(list)->size == CList(list)->limit));
}


ArrayList newArrayList(int size)
{
	List newList = NULL;

	newList = (List)malloc(sizeof(struct List));

	try
	{
		if(newList != NULL)
		{
			newList->ptrlistHead = (ListHead)malloc(sizeof(ListHead));
			newList->ptrListEnd = (ListHead)malloc(sizeof(ListHead));
			if((newList->ptrlistHead != NULL) && (newList->ptrListEnd != NULL))
			{
				*newList->ptrlistHead = NULL;
				*newList->ptrListEnd = NULL;
				if(size < 0)
				{
					newList->property = UNLIMITED;
					newList->size = 0;
				}
				else
				{
					newList->property = LIMITED;
					newList->size = 0;
					newList->limit = size;
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

	return newList;
}


bool isEmpty(ArrayList list)
{
	bool emptyList = false;

	try
	{
		if(list != NULL)
		{
			if(GetListHead(list)!= NULL)
			{
				emptyList = *GetListHead(list) == NULL?true:false;
			}
			else
			{
				throw(__NullPointerException__);
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

	return emptyList;
}

Object removeTopList(ArrayList list)
{
	Object res = NULL;
	Node nodeToBeReleased;

	try
	{
		if(list != NULL)
		{
			if(*GetListHead(list) != NULL)
			{
				nodeToBeReleased = *GetListHead(list);
				*GetListHead(list) = nodeToBeReleased->next;
				if(nodeToBeReleased->next != NULL)
					nodeToBeReleased->next->previous = NULL;
				res = nodeToBeReleased->data;
				free(nodeToBeReleased);
				CList(list)->size -= 1;
				if(*GetListHead(list) == NULL)
				{
					*GetListEnd(list) = NULL;
				} 
			}
			else
			{
				throw(__ListRemoveException__);
			}
		}
		else
		{
			throw(__NullPointerException__);
		}
	}
	catch(ListRemoveException)
	{
		PrintExceptionStdOut(ListRemoveException);
	}
	catch(NullPointerException)
	{
		PrintExceptionStdOut(NullPointerException);
	}

	return res;
}

Object removeBottomList(ArrayList list)
{
	Object res = NULL;
	Node nodeToBeReleased;

	try
	{
		if(list != NULL)
		{
			if(!isEmpty(list))
			{
				nodeToBeReleased = *GetListEnd(list);
				nodeToBeReleased->previous->next = NULL;
				*GetListEnd(list) = nodeToBeReleased->previous;
				res = nodeToBeReleased;
				free(nodeToBeReleased);
				CList(list)->size -= 1;
			}
			else
			{
				throw(__ListRemoveException__);
			}
		}
		else
		{
			throw(__NullPointerException__);
		}
	}
	catch(ListRemoveException)
	{
		PrintExceptionStdOut(ListRemoveException);
	}
	catch(NullPointerException)
	{
		PrintExceptionStdOut(NullPointerException);
	}

	return res;
}

Object removeElementList(ArrayList list, Object data, FunctionCompare compare)
{
	Object res;
	Node nodeToBeReleased;
	Node currentNode;
	Node previousNode;

	try
	{
		if(list != NULL)
		{
			if(!isEmpty(list))
			{
				if(compare(data, (*GetListHead(list))->data ) == 0)
				{
					nodeToBeReleased = *GetListHead(list);
					*GetListHead(list) = nodeToBeReleased->next;
					if(nodeToBeReleased->next != NULL)
						nodeToBeReleased->next->previous = NULL;
					res = nodeToBeReleased->data;
					free(nodeToBeReleased);
				}
				else if(compare(data, (*GetListEnd(list))->data ) == 0)
				{
					nodeToBeReleased = *GetListEnd(list);
					*GetListEnd(list) = nodeToBeReleased->previous;
					nodeToBeReleased->previous->next = NULL;
					res = nodeToBeReleased->data;
					free(nodeToBeReleased);
				}
				else
				{
					currentNode = *GetListHead(list);

					while((currentNode->next != NULL) && (compare(currentNode->data, data)))
					{
						previousNode = currentNode;
						currentNode = currentNode->next;
					}
					if(currentNode == *GetListHead(list))
					{
						*GetListHead(list) = (*GetListHead(list))->next;
						(*GetListHead(list))->next->previous = NULL;
					}
					else
					{
						
						previousNode->next = currentNode->next;
						currentNode->next->previous = previousNode;
						if(currentNode->next == NULL)
							*GetListEnd(list) = previousNode;

					}
					
					free(currentNode);
					res = currentNode->data;
				}

				CList(list)->size -= 1;
			}
			else
			{
				throw(__ListRemoveException__);
			}
		}
		else
		{
			throw(__NullPointerException__);
		}
	}
	catch(ListRemoveException)
	{
		PrintExceptionStdOut(ListRemoveException);
	}
	catch(NullPointerException)
	{
		PrintExceptionStdOut(NullPointerException);
	}

	return res;
}



void insertTopList(ArrayList list, Object data)
{
	Node newNode = NULL;

	try
	{
		if(list != NULL)
		{
			if(!((CList(list)->property == LIMITED) && (CList(list)->size == CList(list)->limit)))
			{
				newNode = (Node)malloc(sizeof(struct Node));
				if(newNode != NULL)
				{
					newNode->previous = NULL;
					newNode->data = data;
					newNode->index = CList(list)->size;
					newNode->next = *GetListHead(list);
					*GetListHead(list) = newNode;
					if(*GetListEnd(list) == NULL)
						*GetListEnd(list) = newNode;

					CList(list)->size += 1;
				}
				else
				{
					throw(__MemoryAllocationException__);
				}
			}
			else
			{
				throw(__ListInsertTopException__);
			}
		}
		else
		{
			throw(__NullPointerException__);
		}
	}
	catch(MemoryAllocationException)
	{
		PrintExceptionStdOut(MemoryAllocationException);
	}
	catch(ListInsertTopException)
	{
		PrintExceptionStdOut(ListInsertTopException);
	}
	catch(NullPointerException)
	{
		PrintExceptionStdOut(NullPointerException);
	}

}

void insertBottomList(ArrayList list, Object data)
{
	Node newNode = NULL;

	try
	{
		if(list != NULL)
		{
			if(!((CList(list)->property == LIMITED) && (CList(list)->size == CList(list)->limit)))
			{
				newNode = (Node)malloc(sizeof(struct Node));
				if(newNode != NULL)
				{
					newNode->next = NULL;
					newNode->data = data;
					newNode->index = CList(list)->size;
					if(*GetListHead(list) == NULL)
					{
						*GetListHead(list) = newNode;
						*GetListEnd(list) = newNode;
						newNode->previous = NULL;
					}
					else
					{
						newNode->previous = *GetListEnd(list);
						newNode->previous->next = newNode;
						*GetListEnd(list) = newNode;
					}

					CList(list)->size += 1;

				}
				else
				{
					throw(__MemoryAllocationException__);
				}
			}
			else
			{
				throw(__ListInsertBottomException__);
			}
		}
		else
		{
			throw(__NullPointerException__);
		}
	}
	catch(MemoryAllocationException)
	{
		PrintExceptionStdOut(MemoryAllocationException);
	}
	catch(ListInsertBottomException)
	{
		PrintExceptionStdOut(ListInsertBottomException);
	}
	catch(NullPointerException)
	{
		PrintExceptionStdOut(NullPointerException);
	}

}

void insertSorted(ArrayList list, Object data, FunctionCompare compare)
{
	Node newNode = NULL;
	Node scroll;
	Node previousNode;

	try
	{
		if(list != NULL)
		{
			if(!checkListLimit(list))
			{
				newNode = (Node)malloc(sizeof(struct Node));
				if(newNode != NULL)
				{
					newNode->data = data;

					if(*GetListHead(list) == NULL)
					{
						newNode->next = *GetListHead(list);
						newNode->previous = NULL;
						*GetListHead(list) = newNode;
						*GetListEnd(list) = newNode;
					}
					else if((*GetListHead(list) != NULL) && (compare(data,(*GetListHead(list))->data) < 0)) 
					{			
						newNode->next = *GetListHead(list);
						newNode->previous = NULL;
						(*GetListHead(list))->previous = newNode;
						*GetListHead(list) = newNode;
					}
					else if(compare(data,(*GetListEnd(list))->data) > 0)
					{
						newNode->next = NULL;
						newNode->previous = *GetListEnd(list);
						(*GetListEnd(list))->next = newNode;
						*GetListEnd(list) = newNode;
					}
					else
					{
						scroll = *GetListHead(list);

						while((scroll != NULL) && (compare(scroll->data, data) < 0))
						{
							previousNode = scroll;
							scroll = scroll->next;
						}
						
						if(scroll == *(GetListHead(list)))
						{
							newNode->next = *GetListHead(list);
							(*GetListHead(list))->previous = newNode;
							newNode->previous = NULL;
							*GetListHead(list) = newNode;
						
						}
						else
						{
							newNode->next = previousNode->next;
							newNode->previous = previousNode;
							previousNode->next = newNode;

							if(scroll != NULL)
							{
								scroll->previous = newNode;
							}							
						}
					}
				}
				else
				{
					throw(__MemoryAllocationException__);
				}

				CList(list)->size++;
			}
			else
			{
				throw(__ListInsertException__);
			}
		}
		else
		{
			throw(__NullPointerException__);
		}
	}
	catch(MemoryAllocationException)
	{
		PrintExceptionStdOut(MemoryAllocationException);
	}
	catch(ListInsertException)
	{
		PrintExceptionStdOut(ListInsertException);
	}
	catch(NullPointerException)
	{
		PrintExceptionStdOut(NullPointerException);
	}
}


void destroyList(ArrayList list)
{
	Node nodeToBeReleased;

	try
	{
		if((list != NULL) && (!isEmpty(list)))
		{
			while(*GetListHead(list) != NULL)
			{
				nodeToBeReleased = *GetListHead(list);
				*GetListHead(list) = (*GetListHead(list))->next;
				free(nodeToBeReleased);
			}
			free(nodeToBeReleased);

			*GetListHead(list) = NULL;
			*GetListEnd(list) = NULL;
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

void destroyArrayList(ArrayList list)
{
	Node nodeToBeReleased;

	try
	{
		if(list != NULL) 
		{
			while(*GetListHead(list) != NULL)
			{
				nodeToBeReleased = *GetListHead(list);
				*GetListHead(list) = (*GetListHead(list))->next;
				free(nodeToBeReleased->data);
				free(nodeToBeReleased);
			}
			free(GetListHead(list));
			free(list);
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

unsigned getListSize(ArrayList list)
{
	try
	{
		if(list != NULL)
		{
			return CList(list)->size;	
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

unsigned getListLimit(ArrayList list)
{
	try
	{
		if(list != NULL)
		{
			return CList(list)->limit;	
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

void printList(ArrayList list, PrintFunctionFormat format)
{
	Node scroll;

	try
	{
		if(list != NULL)
		{
			for(scroll = *GetListHead(list); scroll != NULL;  scroll = scroll->next)
			{
				format(scroll->data);
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

bool fullArrayList(ArrayList list)
{
	bool full = false;
	try
	{
		if(list != NULL)
		{
			full = CList(list)->size == CList(list)->limit?true:false;	
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
	
	return full;
}

Object* listToVector(ArrayList list)
{
	Object* res = NULL;
	Object aux;
	unsigned i = 0;

	try
	{
		if(list != NULL)
		{
			res = (Object)calloc(CList(list)->limit, sizeof(Object));
			if(res != NULL)
			{
				foreach_ArrayList(aux, list)
				{
					res[i] = aux;
					i++;
				}
			}
			else
			{
				throw(__MemoryAllocationException__);
			}
		}
		else
		{
			throw(__NullPointerException__);
		}
	}
	catch(MemoryAllocationException)
	{
		PrintExceptionStdOut(MemoryAllocationException);
	}

	return res;
}