#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/Exception.h"
#include "../include/Ultility.h"
#include "../include/ArrayList.h"
#include "../include/Register.h"
#include "../include/BinaryTree.h"
#include "../include/Client.h"

#define MAX_REGISTERS 1000UL	


extern FILE* PartitionCreate(void);
extern FILE* CreateTestFile(void* tupl);
extern void PartitionClose(FILE* file);
static unsigned partitionsCounter = 0;
static void PrintPartitions(void);
static unsigned readFile(FILE* file);
static int compare(Object obj1, Object obj2);
static bool allFrozen(ArrayList list);
static void unfreezeRecords(ArrayList list);
static void SubstitutionSelection(FILE* file, int mregisters);
static ArrayList GetListGeneratedPartitions(void);
static unsigned HIGH_VALUE;


static int compare(Object obj1, Object obj2)
{
	int res;

	res = (((Register)obj1)->rField.bit.key - ((Register)obj2)->rField.bit.key);

	return res;
}

static bool allFrozen(ArrayList list)
{
	Register temp;
	unsigned counter = 0;
	unsigned listSize;

	listSize = getListSize(list);

	foreach_ArrayList(temp, list)
	{
		if(temp->rField.bit.freeze == true)
		{
			counter += 1;
		}
	}
	

	return counter == listSize?true:false;
}

static void unfreezeRecords(ArrayList list)
{
	Register temp;

	
	foreach_ArrayList(temp, list)
	{
		temp->rField.bit.freeze = false;
	}

}

Register GetRegisterUnfreeze(ArrayList list)
{
	Register temp;

	foreach_ArrayList(temp, list)
	{
		if(temp->rField.bit.freeze == false)
		{
			
			return (Register)removeElementList(list, temp, compare);
		}
	}

	return NULL;
}

static void PrintTestFile(FILE* file)
{
	struct Client client;

	ReWind(file);

	foreachFile(client, file)
	{
		puts("**********************************************");
		printf("Nome: %s\n",client.name);
		printf("Código Cliente: %u\n", client.clientCode);
		printf("Nascimento: %u/%u/%u\n", client.birth.field.day, client.birth.field.month, client.birth.field.year);
		puts("**********************************************");
	}
}

static void SubstitutionSelection(FILE* file, int mregisters)
{
	FILE* partition;
	ArrayList registers;
	struct Register reg;
	struct Client client;
	Register r;
	bool endFile = false;
	volatile int resWrite, resRead = MAX_REGISTERS;

	registers = newArrayList(mregisters);

	ReWind(file);
	partition = PartitionCreate();
	partitionsCounter += 1;

	foreach_mRegisters(client, file, mregisters)
	{	
		reg.rField.bit.key = client.clientCode;
		reg.rField.bit.freeze = false;
		reg.attached = malloc(sizeof(struct Client));
		memcpy(reg.attached, &client, sizeof(struct Client));
		insertSorted(registers, newRegister(reg), compare);
	}
	
	
	while(endFile != true)
	{
		r = GetRegisterUnfreeze(registers);

		if(r != NULL)
		{	
			client.clientCode = r->rField.bit.key;
			memcpy(&client, r->attached, sizeof(struct Client));
			FileWrite(&client, sizeof(struct Client), 1, partition, resWrite);
			FileRead(&client, sizeof(struct Client),1, file, resRead);
			if(resRead > 0)
			{	
				reg.rField.bit.key = client.clientCode;
				if(reg.rField.bit.key < r->rField.bit.key)
				{
					r->rField.bit.freeze = true;
					r->rField.bit.key = reg.rField.bit.key;
					memcpy(r->attached, &client, sizeof(struct Client));
				}
				else
				{
					r->rField.bit.freeze = false;
					r->rField.bit.key = reg.rField.bit.key;
					memcpy(r->attached, &client, sizeof(struct Client));
				}

				insertSorted(registers, r, compare);
			}	
		}
		else if(isEmpty(registers) && resRead == 0)
		{
			endFile = true;
		}
		else if(allFrozen(registers))
		{
			unfreezeRecords(registers);
			PartitionClose(partition);
			partition = PartitionCreate();
			partitionsCounter += 1;
		}
	}
	

	PartitionClose(partition);
	destroyArrayList(registers);
}

static void PrintPartitions(void)
{
	FILE* partition = NULL;
	char bufferStr[64];
	unsigned i;

	for(i = 0; i < partitionsCounter; i++)
	{
		sprintf(bufferStr,"Partitions/Partition %u.bin",i);
		FileOpen(partition, bufferStr, "rb");
		puts(bufferStr);
		//PrintFile(partition);
		PrintTestFile(partition);
		FileClose(partition);
	}	
}

static ArrayList GetListGeneratedPartitions(void)
{
	FILE* partition = NULL;
	char bufferStr[64];
	unsigned i;
	ArrayList partitionList;

	partitionList = newArrayList(partitionsCounter);

	for(i = 0; i < partitionsCounter; i++)
	{
		sprintf(bufferStr,"Partitions/Partition %u.bin",i);
		FileOpen(partition, bufferStr, "rb");
		insertBottomList(partitionList, partition);
	}	

	return partitionList;
}

static unsigned readFile(FILE* file)
{
	struct Client client;
	struct Register reg;
	unsigned res;

	FileRead(&client, sizeof(struct Client), 1, file, res);

	if(res > 0)
		reg.rField.bit.key = client.clientCode;
	else
		reg.rField.bit.key = HIGH_VALUE;
	
	return reg.rField.bit.key;
}

static void writeFile(FILE* partitionFile, FILE* outputFile)
{
	struct Client client;
	int res;

	FileSeek(partitionFile,-sizeof(struct Client), SEEK_CUR);
	FileRead(&client, sizeof(struct Client), 1, partitionFile, res);
	FileWrite(&client, sizeof(struct Client),1, outputFile, res);

}

int main(int argc, char const *argv[])
{
	FILE* file;
	FILE* outFile;
	int nReg,mregisters;

	BinaryTreeWinners tree;
	ArrayList partitionList;
	ArrayList leaves;


	unsigned long long sizeReg = sizeof(struct Client);

	scanf("%i",&nReg);
	scanf("%i",&mregisters);
	void* tupl[3] = {&nReg, CreateAleatoryClient, &sizeReg};

	HIGH_VALUE = nReg;

	file = CreateTestFile(&tupl);

	puts("Arquivo Test");
	PrintTestFile(file);
	
	
	SubstitutionSelection(file, mregisters);
	PrintPartitions();

	tree = newBinaryTreeWinners(4);
	partitionList = GetListGeneratedPartitions();
	outFile = InterweaveTree(tree, partitionList, readFile, writeFile, HIGH_VALUE);
	puts("Arquivo ordenado");
	PrintTestFile(outFile);

	FileClose(file);
	FileClose(outFile);

	return 0;
}