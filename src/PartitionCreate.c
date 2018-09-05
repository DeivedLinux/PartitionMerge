#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/Exception.h"


FILE* PartitionCreate(void)
{
	static unsigned partitionN;
	char bufferStr[64];
	FILE* newPartition = NULL;

	sprintf(bufferStr,"Partitions/Partition %u.bin",partitionN);

	FileOpen(newPartition, bufferStr, "w+b");

	partitionN += 1;

	return newPartition;
}

void PartitionClose(FILE* file)
{
	FileClose(file);
}
