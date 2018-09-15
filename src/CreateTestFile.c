#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/Exception.h"
#include "../include/Register.h"
#include "../include/Client.h"
#include "../include/Ultility.h"
#include "../include/Typedefs.h"



FILE* CreateTestFile(void* tupl)
{
	FILE* newTestFile = NULL;
	unsigned i;
	int n;
	void(*func)(Object, int);
	void(*format)(Object);
	unsigned offset;
	volatile int res;
	void* blockMemory;

	FileOpen(newTestFile, "TestFile.bin", "w+b");

	srand(time(NULL));
	n = *((int*)(((void**)tupl)[0]));
	func = ((void(*)(Object,int))(((void**)tupl)[1]));
	offset = *((unsigned long*)(((void**)tupl)[2]));

	blockMemory = calloc(offset, sizeof(Byte));

	ReWind(newTestFile);
	
	for(i = 0; i < n; i++)
	{
		func(blockMemory,n);
		FileWrite(blockMemory, offset, 1, newTestFile, res);
	}

	free(blockMemory);
	

	return newTestFile;
}

