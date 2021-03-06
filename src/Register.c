#include "../include/Register.h"
#include "../include/Exception.h"
#include "../include/Ultility.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


void PrintFile(FILE* file)
{
	struct Register reg;
	
	ReWind(file);

	foreachFile(reg, file)
	{
		printf("key: %u\n", reg.rField.bit.key);
	}	
}

void CreateAleatoryRegister(Register outRegister, int n)
{
	struct Register res;

	res.rField.bit.key = rand()%n;
	printf("aaa%i\n",n );
	res.rField.bit.freeze = false;
	*outRegister = res;
}

Register newRegister(struct Register res)
{
	Register newReg = NULL;

	try
	{
		newReg = calloc(1, sizeof(struct Register));
		if(newReg != NULL)
		{
			newReg->rField.bit.key = res.rField.bit.key;
			newReg->rField.bit.freeze = res.rField.bit.freeze;
			newReg->attached = res.attached;
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

	return newReg;
}