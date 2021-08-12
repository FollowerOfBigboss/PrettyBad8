#ifndef DYNAREC
#define DYNAREC

// Concept

struct CodeBlock
{
	void* code;
	int size;
};


void* AllocateDynarecMem(int size);

#endif
