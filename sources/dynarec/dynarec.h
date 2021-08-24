#ifndef DYNAREC
#define DYNAREC

#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

struct CodeBlock
{
	void* code;
	uint32_t size;
};


bool AllocateDynarecMem(CodeBlock* block, uint32_t size);
bool DeAllocateDynarecMem(CodeBlock* block);

#endif
