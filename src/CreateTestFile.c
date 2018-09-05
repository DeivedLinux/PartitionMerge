#include <stdio.h>
#include <stdlib.h>
#include "../include/Exception.h"
#include "../include/Register.h"



FILE* CreateTestFile(void* tupl)
{
	FILE* newTestFile = NULL;
	unsigned i;
	int n;
	void(*func)(void*, int);
	unsigned offset;
	struct Register reg;
	volatile int res;

	FileOpen(newTestFile, "TestFile.bin", "w+b");

	n = *((int*)(((void**)tupl)[0]));
	func = ((void(*)(void*,int))(((void**)tupl)[1]));
	offset = *((unsigned long*)(((void**)tupl)[2]));

	ReWind(newTestFile);
	
	for(i = 0; i < n; i++)
	{
		func(&reg,n);
		FileWrite(&reg, offset, 1, newTestFile, res);
	}
	

	return newTestFile;
}

