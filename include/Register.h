#ifndef REGISTER_H_
#define REGISTER_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct Register
{
	union
	{
		unsigned int inf;
		struct 
		{
			unsigned key:31;
			unsigned freeze:1;
		}bit;
	}rField;
	void* info;
}*Register;

void PrintFile(FILE* file);
void CreateAleatoryRegister(Register outRegister, int n);
Register newRegister(struct Register res);





#endif